#include "gimbal_task.h"

TaskHandle_t GIMBAL_Task_Handler;
void gimbal_task(void *p_arg);
Gimbal_Mode_Enum Gimbal_Mode;

void Gimbal_Stop_Control();
void Gimbal_Follow_Control();
void Gimbal_AutoAim_Control();

/**
 *@Function:		gimbal_task_create()
 *@Description:	云台任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void gimbal_task_create()
{
		xTaskCreate((TaskFunction_t )gimbal_task,          //任务函数
							(const char*    )"gimbal_task",          //任务名称
							(uint16_t       )GIMBAL_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )GIMBAL_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&GIMBAL_Task_Handler);  //任务句柄  
}
/**
 *@Function:		gimbal_task(void *p_arg)
 *@Description:	云台任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void gimbal_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(2);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	while(1)
	{
		switch(Gimbal_Mode)
		{
			//云台跟随模式
			case GIMBAL_MODE_FOLLOW:
				Gimbal_Follow_Control();
				break;
			//云台自瞄模式
			case GIMBAL_MODE_AUTOAIM:
				Gimbal_AutoAim_Control();
				break;
			//云台无力模式
			case GIMBAL_MODE_STOP:
				Gimbal_Stop_Control();
				break;
			//云台打符模式
			case GIMBAL_MODE_RUNE:
				Gimbal_Stop_Control();
				break;
			default:
				break;
		}
		
		xEventGroupSetBits(EventGroupHandler,GIMBAL_SIGNAL);	//云台事件组置位
		xEventGroupSetBits(EventGroupHandler,SHOOT_EXE_SIGNAL);	//发射任务处理置位
		
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}
float Yaw_Aim_Angle = 0;
float Pitch_Aim_Angle;
float Pitch_Angle_Max;
float Pitch_Angle_Min;
/**
 *@Function:		Gimbal_Follow_Control()
 *@Description:	云台跟随控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Gimbal_Follow_Control()
{
		Pitch_Aim_Angle += DBUS.RC.ch3 / 50.0f - DBUS.PC.Y / 10.0f ;
	
		if(Pitch_Aim_Angle > Pitch_Angle_Max)
		{Pitch_Aim_Angle = Pitch_Angle_Max;}
		else if(Pitch_Aim_Angle < Pitch_Angle_Min)
		{Pitch_Aim_Angle = Pitch_Angle_Min;}			
			
		PID_Gimbal_Angle[1].PIDout = Pid_Calc(&PID_Gimbal_Angle[1],CAN_Gimbal[1].Current_MechAngle,Pitch_Aim_Angle);
		CAN_Gimbal[1].Target_Current = Pid_Calc(&PID_Gimbal_Speed[1],-mpu_data.gy*0.1f,PID_Gimbal_Angle[1].PIDout);

		
		Yaw_Aim_Angle -=DBUS.RC.ch2/600.0f + DBUS.PC.X/100.0f;
		
		if(DBUS.PC.Keyboard & KEY_Q)
		{
			Yaw_Aim_Angle += 0.3;
		}
		else if(DBUS.PC.Keyboard & KEY_E)
		{
			Yaw_Aim_Angle -= 0.3;			
		}
		
		PID_Gimbal_Angle[0].PIDout = Pid_Calc(&PID_Gimbal_Angle[0],Yaw_Angle,Yaw_Aim_Angle);
		CAN_Gimbal[0].Target_Current = Pid_Calc(&PID_Gimbal_Speed[0],mpu_data.gz*0.1f,PID_Gimbal_Angle[0].PIDout);
}

/**
 *@Function:		Gimbal_AutoAim_Control()
 *@Description:	云台自瞄控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Gimbal_AutoAim_Control()
{
		Pitch_Aim_Angle += DBUS.RC.ch3 / 50.0f - DBUS.PC.Y / 10.0f;
		Pitch_Aim_Angle +=NUC_Data.pit_offset / 1.0f;
	
	
		if(Pitch_Aim_Angle > Pitch_Angle_Max)
		{Pitch_Aim_Angle = Pitch_Angle_Max;}
		else if(Pitch_Aim_Angle < Pitch_Angle_Min)
		{Pitch_Aim_Angle = Pitch_Angle_Min;}			
			
		PID_Gimbal_Angle[1].PIDout = Pid_Calc(&PID_Gimbal_Angle[1],CAN_Gimbal[1].Current_MechAngle,Pitch_Aim_Angle);
		CAN_Gimbal[1].Target_Current = Pid_Calc(&PID_Gimbal_Speed[1],-mpu_data.gy*0.1f,PID_Gimbal_Angle[1].PIDout);
		
		Yaw_Aim_Angle = Yaw_Angle;
		Yaw_Aim_Angle -=DBUS.RC.ch2/600.0f + DBUS.PC.X/100.0f;
		Yaw_Aim_Angle +=NUC_Data.yaw_offset;
		
		PID_Gimbal_Angle[0].PIDout = Pid_Calc(&PID_Gimbal_Angle[0],Yaw_Angle,Yaw_Aim_Angle);
		CAN_Gimbal[0].Target_Current = Pid_Calc(&PID_Gimbal_Speed[0],mpu_data.gz*0.1f,PID_Gimbal_Angle[0].PIDout);
}
/**
 *@Function:		Gimbal_Stop_Control()
 *@Description:	云台无力控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Gimbal_Stop_Control()
{
	//目标电流0，无力
	CAN_Gimbal[0].Target_Current = 0;
	CAN_Gimbal[1].Target_Current = 0;
}