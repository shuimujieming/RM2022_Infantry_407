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
 *@Description:	�������񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void shoot_task_create()
{
		xTaskCreate((TaskFunction_t )shoot_task,          //������
							(const char*    )"shoot_task",          //��������
							(uint16_t       )SHOOT_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )SHOOT_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&SHOOT_Task_Handler);  //������  
}

/**
 *@Function:		shoot_task(void *p_arg)
 *@Description:	��������
 *@Param:       �β�
 *@Return:	  	����ֵ
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
			//����ֹͣ
			case SHOOT_MODE_STOP:
				Shoot_Stop_Control();
				break;
			//��������
			case SHOOT_MODE_RUN:
				Shoot_Run_Control();
				break;

			default:
				break;
		}

		xEventGroupSetBits(EventGroupHandler,SHOOT_SIGNAL);	//�����¼�����λ

	}

}
/**
 *@Function:		Shoot_Stop_Control()
 *@Description:	������������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Shoot_Stop_Control()
{
	Shoot_Cmd = SHOOT_CMD_STOP;//ֹͣ����

	CAN_Trigger.Target_Current = Pid_Calc(&PID_Trigger,CAN_Trigger.Current_Speed,0);
	CAN_Shoot[0].Target_Current = Pid_Calc(&PID_Shoot[0],CAN_Shoot[0].Current_Speed,0);
	CAN_Shoot[1].Target_Current = Pid_Calc(&PID_Shoot[1],CAN_Shoot[1].Current_Speed,0);		
}


extern int Trig_Time ;
volatile float shoot_speed_get = 0;
int Shoot_Speed = 0;

//7000����30m/s
//4500����15m/s
int shoot_bullet_speed = 7000;

extern int Shoot_Once_Flag;
extern int Shoot_Aim_Angle;

//��ת���ڼ�ʱ
static int Block_Time = 0;
//��תʱ���ʱ
static int Block_Reverse_Time = 0;

/**
 *@Function:		Shoot_Run_Control()
 *@Description:	�������п���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Shoot_Run_Control()
{
	//�����ж�
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
	
	
	//����ģʽ
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
	//����ģʽ
	else if(Shoot_Cmd == SHOOT_CMD_LONG)
	{
		//�жϲ����Ƿ�λ���·�����������һֱ��ס
		if(Trig_Time == 0)
		{
			Shoot_Speed = 0;
			Shoot_Cmd = SHOOT_CMD_STOP;
		}
		else
		{
			//����Ҫ�󷢵��ȶ�
			//5500
			
			Shoot_Speed = 5500;
		}
	}
	else
	{
			Shoot_Speed = 0;		
	}
	
	//���俨������
	Bullet_Block_Control();
	
	//�������ƣ���ֹ���������������ŵ�����������֤����Ƶ�²�������
	if(game_robot_state.shooter_id1_17mm_cooling_limit < power_heat_data.shooter_id1_17mm_cooling_heat + 30)
	{
		Shoot_Speed = 0;
	}
	
	//���ٻ�ȡ
	shoot_speed_get = real_shoot_data.bullet_speed;

	//������ת�ٴ���
	if(Shoot_Cmd != SHOOT_CMD_ONCE)
	CAN_Trigger.Target_Current = Pid_Calc(&PID_Trigger,CAN_Trigger.Current_Speed,Shoot_Speed);			
	
	//Ħ����ת�ٴ���
	CAN_Shoot[0].Target_Current = Pid_Calc(&PID_Shoot[0],CAN_Shoot[0].Current_Speed,-shoot_bullet_speed);
	CAN_Shoot[1].Target_Current = Pid_Calc(&PID_Shoot[1],CAN_Shoot[1].Current_Speed,shoot_bullet_speed);

}
/**
 *@Function:		Bullet_Block_Control()
 *@Description:	��������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Bullet_Block_Control()
{

	
	//�����������ʱ��ⷴת
	if(CAN_Trigger.Current > 10000)
	{
		Block_Time ++;
	}
	

	
	//��ת
	if(Block_Reverse_Time >= 1)
	{
		Block_Reverse_Time++;
		
		//��תʱ�� 3 * 2ms = 6ms
		if(Block_Reverse_Time > 20)
		{
			Block_Reverse_Time = 0;
			Block_Time = 0;
		}
		else
		{
			//��תʱ������Сʱ������ת����ֹ��ת��ת
			if(CAN_Trigger.Current > -4000)
			{Shoot_Speed = -4000;}
			//�����ϴ�ָ���ת
			else
			{
				Block_Reverse_Time = 0;
				Block_Time = 0;
			}
		}
	}
	else
	{
		//��תʱ��10*�����������ڣ�2ms��= 20ms
		if(Block_Time == 10)
		{
			Block_Reverse_Time = 1;
		}		
	}
	
}