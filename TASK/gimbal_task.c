#include "gimbal_task.h"

TaskHandle_t GIMBAL_Task_Handler;
void gimbal_task(void *p_arg);
Gimbal_Mode_Enum Gimbal_Mode;

void Gimbal_Stop_Control();
void Gimbal_Follow_Control();
void Gimbal_AutoAim_Control();

/**
 *@Function:		gimbal_task_create()
 *@Description:	��̨���񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void gimbal_task_create()
{
		xTaskCreate((TaskFunction_t )gimbal_task,          //������
							(const char*    )"gimbal_task",          //��������
							(uint16_t       )GIMBAL_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )GIMBAL_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&GIMBAL_Task_Handler);  //������  
}
/**
 *@Function:		gimbal_task(void *p_arg)
 *@Description:	��̨����
 *@Param:       �β�
 *@Return:	  	����ֵ
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
			//��̨����ģʽ
			case GIMBAL_MODE_FOLLOW:
				Gimbal_Follow_Control();
				break;
			//��̨����ģʽ
			case GIMBAL_MODE_AUTOAIM:
				Gimbal_AutoAim_Control();
				break;
			//��̨����ģʽ
			case GIMBAL_MODE_STOP:
				Gimbal_Stop_Control();
				break;
			//��̨���ģʽ
			case GIMBAL_MODE_RUNE:
				Gimbal_Stop_Control();
				break;
			default:
				break;
		}
		
		xEventGroupSetBits(EventGroupHandler,GIMBAL_SIGNAL);	//��̨�¼�����λ
		xEventGroupSetBits(EventGroupHandler,SHOOT_EXE_SIGNAL);	//������������λ
		
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}
float Yaw_Aim_Angle = 0;
float Pitch_Aim_Angle;
float Pitch_Angle_Max;
float Pitch_Angle_Min;
/**
 *@Function:		Gimbal_Follow_Control()
 *@Description:	��̨�������
 *@Param:       �β�
 *@Return:	  	����ֵ
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
 *@Description:	��̨�������
 *@Param:       �β�
 *@Return:	  	����ֵ
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
 *@Description:	��̨��������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Gimbal_Stop_Control()
{
	//Ŀ�����0������
	CAN_Gimbal[0].Target_Current = 0;
	CAN_Gimbal[1].Target_Current = 0;
}