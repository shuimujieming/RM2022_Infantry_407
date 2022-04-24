#include "shoot_task.h"

Shoot_Mode_Enum Shoot_Mode;
Shoot_Cmd_Enum Shoot_Cmd = SHOOT_CMD_STOP;

void Shoot_Stop_Control();
void Shoot_Run_Control();
void Bullet_Block_Control();

TaskHandle_t SHOOT_Task_Handler;
void shoot_task(void *p_arg);


/**
 *@Function:		shoot_task_create()
 *@Description:	发射任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void shoot_task_create()
{
		xTaskCreate((TaskFunction_t )shoot_task,          //任务函数
							(const char*    )"shoot_task",          //任务名称
							(uint16_t       )SHOOT_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )SHOOT_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&SHOOT_Task_Handler);  //任务句柄  
}

/**
 *@Function:		shoot_task(void *p_arg)
 *@Description:	发射任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void shoot_task(void *p_arg)
{
		EventBits_t  EventValue;

	while(1)
	{
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)EventGroupHandler,
																			(EventBits_t      )SHOOT_EXE_SIGNAL,
																			(BaseType_t       )pdTRUE,
																			(BaseType_t       )pdFALSE,
																			(TickType_t       )portMAX_DELAY);
		switch(Shoot_Mode)
		{
			//发射停止
			case SHOOT_MODE_STOP:
				Shoot_Stop_Control();
				break;
			//发射运行
			case SHOOT_MODE_RUN:
				Shoot_Run_Control();
				break;

			default:
				break;
		}

		xEventGroupSetBits(EventGroupHandler,SHOOT_SIGNAL);	//发射事件组置位

	}

}
/**
 *@Function:		Shoot_Stop_Control()
 *@Description:	发射无力控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Shoot_Stop_Control()
{
	Shoot_Cmd = SHOOT_CMD_STOP;//停止发射

	CAN_Trigger.Target_Current = Pid_Calc(&PID_Trigger,CAN_Trigger.Current_Speed,0);
	CAN_Shoot[0].Target_Current = Pid_Calc(&PID_Shoot[0],CAN_Shoot[0].Current_Speed,0);
	CAN_Shoot[1].Target_Current = Pid_Calc(&PID_Shoot[1],CAN_Shoot[1].Current_Speed,0);		
}


extern int Trig_Time ;
volatile float shoot_speed_get = 0;
int Shoot_Speed = 0;

//7000——30m/s
//4500——15m/s
int shoot_bullet_speed = 7000;

extern int Shoot_Once_Flag;
extern int Shoot_Aim_Angle;

//堵转周期计时
static int Block_Time = 0;
//反转时间计时
static int Block_Reverse_Time = 0;

/**
 *@Function:		Shoot_Run_Control()
 *@Description:	发射运行控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Shoot_Run_Control()
{
	//射速判断
	if(game_robot_state.shooter_id1_17mm_speed_limit == 15)
	{
		shoot_bullet_speed = 4500;
	}
	else if(game_robot_state.shooter_id1_17mm_speed_limit == 30)
	{
		shoot_bullet_speed = 7000;		
	}
	else
	{
		shoot_bullet_speed = 4500;		
	}
	
	
	//单发模式
	if(Shoot_Cmd == SHOOT_CMD_ONCE)
	{
		
		if(Shoot_Once_Flag)
		{
			if(Block_Reverse_Time == 0)
			{
				PID_Trigger_Angle.PIDout = Pid_Calc(&PID_Trigger_Angle,CAN_Trigger.Total_MechAngle,Shoot_Aim_Angle);
				CAN_Trigger.Target_Current = Pid_Calc(&PID_Trigger_Speed,CAN_Trigger.Current_Speed,PID_Trigger_Angle.PIDout);
				if(fabs(PID_Trigger_Angle.CurrentError) < 10)
				{
					Shoot_Once_Flag = 0;
					Shoot_Cmd = SHOOT_CMD_STOP;
				}				
			}
			else
			{
					CAN_Trigger.Target_Current = Pid_Calc(&PID_Trigger,CAN_Trigger.Current_Speed,Shoot_Speed);			
			}
		}
		
	}
	//连发模式
	else if(Shoot_Cmd == SHOOT_CMD_LONG)
	{
		//判断拨杆是否位于下方或者鼠标左键一直按住
		if(Trig_Time == 0)
		{
			Shoot_Speed = 0;
			Shoot_Cmd = SHOOT_CMD_STOP;
		}
		else
		{
			//连发要求发弹稳定
			//5500
			
			Shoot_Speed = 5500;
		}
	}
	else
	{
			Shoot_Speed = 0;		
	}
	
	//发射卡弹控制
	Bullet_Block_Control();
	
	//热量限制（防止超热量）留出三颗弹丸余量，保证高射频下不超热量
	if(game_robot_state.shooter_id1_17mm_cooling_limit < power_heat_data.shooter_id1_17mm_cooling_heat + 30)
	{
		Shoot_Speed = 0;
	}
	
	//射速获取
	shoot_speed_get = real_shoot_data.bullet_speed;

	//拨弹盘转速处理
	if(Shoot_Cmd != SHOOT_CMD_ONCE)
	CAN_Trigger.Target_Current = Pid_Calc(&PID_Trigger,CAN_Trigger.Current_Speed,Shoot_Speed);			
	
	//摩擦轮转速处理
	CAN_Shoot[0].Target_Current = Pid_Calc(&PID_Shoot[0],CAN_Shoot[0].Current_Speed,-shoot_bullet_speed);
	CAN_Shoot[1].Target_Current = Pid_Calc(&PID_Shoot[1],CAN_Shoot[1].Current_Speed,shoot_bullet_speed);

}
/**
 *@Function:		Bullet_Block_Control()
 *@Description:	卡弹控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Bullet_Block_Control()
{

	
	//电机电流过大时检测反转
	if(CAN_Trigger.Current > 10000)
	{
		Block_Time ++;
	}
	

	
	//反转
	if(Block_Reverse_Time >= 1)
	{
		Block_Reverse_Time++;
		
		//反转时间 3 * 2ms = 6ms
		if(Block_Reverse_Time > 20)
		{
			Block_Reverse_Time = 0;
			Block_Time = 0;
		}
		else
		{
			//反转时电流较小时才允许反转，防止反转堵转
			if(CAN_Trigger.Current > -4000)
			{Shoot_Speed = -4000;}
			//电流较大恢复正转
			else
			{
				Block_Reverse_Time = 0;
				Block_Time = 0;
			}
		}
	}
	else
	{
		//堵转时间10*发射任务周期（2ms）= 20ms
		if(Block_Time == 10)
		{
			Block_Reverse_Time = 1;
		}		
	}
	
}