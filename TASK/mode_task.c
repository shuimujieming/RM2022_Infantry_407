#include "mode_task.h"


void Get_Chassis_Mode();
void Get_Gimbal_Mode();
void Get_Chassis_RC();
void Get_Chassis_Keyboard();
void Get_Shoot_Mode();
void Get_Cover_Mode();
void Get_Keyboard_Mode();

//ң����˥���ź�
extern int DR16_Signal;

extern Chassis_Mode_Enum Chassis_Mode;
extern Gimbal_Mode_Enum Gimbal_Mode;
extern Shoot_Mode_Enum Shoot_Mode;

extern Chassis_Control_Speed_Typedef rc;
extern Chassis_Control_Speed_Typedef keyboard;	


TaskHandle_t MODE_Task_Handler;
void mode_task(void *p_arg);

/**
 *@Function:		mode_task_create()
 *@Description:	ģʽ�л����񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void mode_task_create()
{
		xTaskCreate((TaskFunction_t )mode_task,          //������
							(const char*    )"mode_task",          //��������
							(uint16_t       )MODE_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )MODE_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&MODE_Task_Handler);  //������  
}




extern int CAN1_Signal ;
extern int CAN2_Signal ;
extern int SuperCap_Signal ;

/**
 *@Function:		mode_task(void *p_arg)
 *@Description:	ģʽ�л�����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void mode_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(2);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	while(1)
	{		
		//�ź�˥��
		CAN1_Signal     --;
		CAN2_Signal     --;
		SuperCap_Signal --;
		DR16_Signal     --; //�ź����ݼ����ڴ�֮ǰ��ֵӦ�ò�Ϊ0,������Ϊң�����ϵ�
		//ң���������������˹رտ���
		if(DR16_Signal <= 0)
		{
			Chassis_Mode = CHASSIS_MODE_STOP;
			Gimbal_Mode = GIMBAL_MODE_STOP;
			Shoot_Mode = SHOOT_MODE_STOP;
			DR16_Signal = 0;
		}
		//ң���������У���������
		else
		{
			//�����ٶȻ�ȡ
			Get_Chassis_RC();
			Get_Chassis_Keyboard();
			
			//������ģʽ״̬����
			Get_Keyboard_Mode();
			Get_Gimbal_Mode();
			Get_Chassis_Mode();
			Get_Shoot_Mode();
			Get_Cover_Mode();			
		}
		
		//CAN2����ʱĦ����ͣ
		if(CAN2_Signal <= 0)
		{
			Shoot_Mode = SHOOT_MODE_STOP;
		}
		
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}
/**
 *@Function:		Get_Chassis_RC()
 *@Description:	��ȡ����ң�����ٶ�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Get_Chassis_RC()
{
	//�ֶ���������ٶ�
	rc.vx = DBUS.RC.ch1 / 660.0f * 5000.0f;
	rc.vy = DBUS.RC.ch0 / 660.0f * 5000.0f;
	rc.vw = DBUS.RC.ch2 / 660.0f * 1500.0f;
	
}
ramp_t fb_ramp = RAMP_GEN_DAFAULT;
ramp_t lr_ramp = RAMP_GEN_DAFAULT;
ramp_t rotate_ramp = RAMP_GEN_DAFAULT;
ramp_t slow_ramp = RAMP_GEN_DAFAULT;
ramp_t close_ramp = RAMP_GEN_DAFAULT;
ramp_t slow_fb_ramp = RAMP_GEN_DAFAULT;
ramp_t slow_rl_ramp = RAMP_GEN_DAFAULT;
/**
 *@Function:		Get_Chassis_Keyboard()
 *@Description:	��ȡ���̼����ٶ�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Get_Chassis_Keyboard()
{
	static float current_speed_x = 0;
	static float current_speed_y = 0;	
	
	//ǰ������
	if(DBUS.PC.Keyboard & KEY_W)
	{
		keyboard.vx = current_speed_x + (6000 - current_speed_x) * ramp_calc(&fb_ramp);		
		ramp_init(&slow_ramp,2000);			//2000	
	}
	else if(DBUS.PC.Keyboard & KEY_S)
	{
		keyboard.vx = current_speed_x + (-6000 - current_speed_x) * ramp_calc(&fb_ramp);	
		ramp_init(&slow_ramp,2000);
	}
	else
	{
    keyboard.vx = current_speed_x * (1-ramp_calc(&slow_ramp));
    ramp_init(&fb_ramp, 2000);
	}
	
	//����ƽ��
	if(DBUS.PC.Keyboard & KEY_A)
	{
		keyboard.vy = current_speed_y + (-5000 -current_speed_y) * ramp_calc(&lr_ramp);			
		ramp_init(&slow_ramp,2000);
	}
	else if(DBUS.PC.Keyboard & KEY_D)
	{
		keyboard.vy = current_speed_y +(5000 -current_speed_y) * ramp_calc(&lr_ramp);
		ramp_init(&slow_ramp,2000);
	}
	else
	{
		keyboard.vy = 0;
    ramp_init(&lr_ramp, 2000);
	}
		current_speed_x = keyboard.vx;
		current_speed_y = keyboard.vy;
}
extern	char Send_Once_Flag;//��ʼ����־
int Cover_Open_Flag = 0; //���̷��ǿ���״̬��־λ
int Shoot_Run_Flag = 0; //����Ħ���ֿ���״̬��־λ
int Chassis_Rotate_Flag = 0; //����С���ݿ���״̬��־λ
/**
 *@Function:		Get_Keyboard_Mode()
 *@Description:	���̻�ȡģʽ
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Get_Keyboard_Mode()
{
	//���̿��Ƶ��ָǿ���
	if(DBUS.PC.Keyboard & KEY_X && DBUS.PC.Keyboard & KEY_CTRL)
	{
		Cover_Open_Flag --;
	}
	else if(DBUS.PC.Keyboard & KEY_X)
	{
		Cover_Open_Flag ++;
	}
	else
	{
		Cover_Open_Flag = 0;
	}
	
	
	//���̿���Ħ���ֿ���
	if(DBUS.PC.Keyboard & KEY_V && DBUS.PC.Keyboard & KEY_CTRL)
	{
		Shoot_Run_Flag --;
	}
	else if(DBUS.PC.Keyboard & KEY_V)
	{
		//UI���·���
		Send_Once_Flag = 0;
		Shoot_Run_Flag ++;
	}
	else
	{
		Shoot_Run_Flag = 0;
	}
	
	//���̿��Ƶ���ģʽ����
	if(DBUS.PC.Keyboard & KEY_C && DBUS.PC.Keyboard & KEY_CTRL)
	{
		Chassis_Rotate_Flag --;
	}
	else if(DBUS.PC.Keyboard & KEY_C)
	{
		Chassis_Rotate_Flag ++;
	}
	else
	{
		Chassis_Rotate_Flag = 0;
	}
}
//����״̬��־λ
typedef enum
{
	CHASSIS_STATUS_STOP = 0,
	CHASSIS_STATUS_ROTATE,
	CHASSIS_STATUS_FOLLOW,

}Chassis_Status_Enum;
//����״̬�������ã�
Chassis_Status_Enum Chassis_Status = CHASSIS_STATUS_FOLLOW;

/**
 *@Function:		Get_Chassis_Mode()
 *@Description:	��ȡ����ģʽ
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Get_Chassis_Mode()
{	
	//˫�µ����ٶ�Ϊ0
	if(DBUS.RC.Switch_Left == RC_SW_DOWN && DBUS.RC.Switch_Right == RC_SW_DOWN)
	{
		Chassis_Mode = CHASSIS_MODE_STOP;
	}
	else
	{
		
		//���̵������ݿ��ؿ���
		if(Chassis_Rotate_Flag > 50)//50*2 = 100ms
		{
			Chassis_Status = CHASSIS_STATUS_ROTATE;
			Chassis_Rotate_Flag = 0;
		}
		else if(Chassis_Rotate_Flag < -50)
		{
			Chassis_Status = CHASSIS_STATUS_FOLLOW;
			Chassis_Rotate_Flag = 0;		
		}	
		
		//ң��������ģʽ����
		if(DBUS.RC.Switch_Right == RC_SW_UP || Chassis_Status == CHASSIS_STATUS_FOLLOW)
		{
			Chassis_Mode = CHASSIS_MODE_FOLLOW;
		}
		
		if(DBUS.RC.Switch_Right == RC_SW_DOWN || Chassis_Status == CHASSIS_STATUS_ROTATE)
		{
			Chassis_Mode = CHASSIS_MODE_ROTATE;
		}
		
		if(DBUS.RC.Switch_Right == RC_SW_MID)
		{
			Chassis_Mode = CHASSIS_MODE_STOP;
		}
	
	}

}
/**
 *@Function:		Get_Gimbal_Mode()
 *@Description:	��ȡ��̨ģʽ
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Get_Gimbal_Mode()
{
	//˫������
	if(DBUS.RC.Switch_Left == RC_SW_DOWN && DBUS.RC.Switch_Right == RC_SW_DOWN)
	{
		Gimbal_Mode = GIMBAL_MODE_STOP;
	}
	else
	{
		//������̨ģʽ��ȡ
		if(DBUS.RC.Switch_Right == RC_SW_UP)
		{
			Gimbal_Mode = GIMBAL_MODE_FOLLOW;
		}
		
		//�Ҽ���ס���飬�����Ҳ�������
		if(DBUS.RC.Switch_Right == RC_SW_MID || DBUS.PC.Press_Right)
		{
			Gimbal_Mode = GIMBAL_MODE_AUTOAIM;
		}
		
		//���µ���ģʽΪ��ת����ʱ��̨���ָ���ģʽ
		else if(DBUS.RC.Switch_Right == RC_SW_DOWN)
		{
			Gimbal_Mode = GIMBAL_MODE_FOLLOW;
		}		
	}
}


//���ָǷ�ת��־λ
int Cover_Ready_Flag = 0; //ң����
/**
 *@Function:		Get_Cover_Mode()
 *@Description:	��ȡ����ģʽ
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Get_Cover_Mode()
{	
	//���ǿ��ؿ���
	if(Cover_Open_Flag > 50)//50*2 = 100ms
	{
		COVER_OFF;
		Cover_Open_Flag = 0;
	}
	else if(Cover_Open_Flag < -50)
	{
		COVER_ON;
		Cover_Open_Flag = 0;		
	}
	
	//���ֿ��Ƶ��ָǣ����²���������һ��λ�ú���һ��״̬
	if(DBUS.RC.ch4 > 600)
	{
		Cover_Ready_Flag = 1;
	}
	if(Cover_Ready_Flag == 1)
	{
		//�������ӽ��м�λ��
		if(DBUS.RC.ch4 < 100 && DBUS.RC.ch4 > 0)
		{
			
			if(TIM1->CCR1 == COVER_CLOSE_VAL)
			{
				TIM1->CCR1 = COVER_OPEN_VAL;
			}
			else if(TIM1->CCR1 == COVER_OPEN_VAL)
			{
				TIM1->CCR1 = COVER_CLOSE_VAL;
			}
			Cover_Ready_Flag = 0;
		}
	}

}

int Trig_Time = 0;	//���䴥��ʱ��
int Switch_Left_Last = RC_SW_UP; //�󲦸���һ״̬
extern Shoot_Cmd_Enum Shoot_Cmd; //����ģʽ������1 ����2
int Shoot_Once_Flag = 0;
int Shoot_Aim_Angle = 0;
int Shoot_Angle_Bullet = 36856;
/**
 *@Function:		Get_Shoot_Mode()
 *@Description:	��ȡ����ģʽ
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Get_Shoot_Mode()
{
	//˫������
	if(DBUS.RC.Switch_Left == RC_SW_DOWN && DBUS.RC.Switch_Right == RC_SW_DOWN)
	{
		Shoot_Mode = SHOOT_MODE_STOP;
	}
	else
	{
		//���俪�ؿ���
		if(Shoot_Run_Flag > 50)//50*2 = 100ms
		{
			Shoot_Mode = SHOOT_MODE_RUN;
			Shoot_Run_Flag = 0;
		}
		else if(Shoot_Run_Flag < -50)
		{
			Shoot_Mode = SHOOT_MODE_STOP;
			Shoot_Run_Flag = 0;		
		}
		
		//ң��������Ħ���ֿ���
		if(Switch_Left_Last == RC_SW_MID)
		{
			if(DBUS.RC.Switch_Left == RC_SW_UP)
			{
				if(Shoot_Mode == SHOOT_MODE_RUN)
				{Shoot_Mode = SHOOT_MODE_STOP;}
				else if(Shoot_Mode == SHOOT_MODE_STOP)
				{Shoot_Mode = SHOOT_MODE_RUN;}			
			}
		}
		Switch_Left_Last = DBUS.RC.Switch_Left;
		
		//���ڿ���Ħ���ֵ��������Ч
		if(Shoot_Mode == SHOOT_MODE_RUN)
		{
			//ң�������Ƶ�������
			if(DBUS.RC.Switch_Left == RC_SW_DOWN || DBUS.PC.Press_Left)
			{
				Trig_Time ++;
			}
			else
			{
				Trig_Time = 0;
			}
			//�̰�2ms
			if(Trig_Time == 1)
			{
				Shoot_Cmd = SHOOT_CMD_ONCE;
				Shoot_Aim_Angle = CAN_Trigger.Total_MechAngle + Shoot_Angle_Bullet;
				
				//������־
				Shoot_Once_Flag = 1;
			}
			//����200ms
			else if(Trig_Time > 100)
			{
				Shoot_Cmd = SHOOT_CMD_LONG;
			}
			
		}		
	}
	
}