#include "gimbal.h"

//Flash��Ϣ�����ַ
#define CONFIG_PARAM_SIZE	(1020*1024)
#define CONFIG_PARAM_ADDR 	(FLASH_BASE + CONFIG_PARAM_SIZE)	

Init_Angle_Data Angle_Init;

/**
 *@Function:		Angle_Correct_Get(int angle_to_resize , int angle_critical)
 *@Description:	�ǶȾ���ֵ��ȡ�����ص�ǰCriticalֵ��������ʵ�Ƕ�ֵ
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
static int Angle_Correct_Get(int angle_to_resize , int angle_critical)
{	
		if(angle_to_resize < angle_critical)
		{
			return angle_to_resize + 8192;
		}
		else
		{
			return angle_to_resize;
		}	
}

/**
 *@Function:		Encoder_Data_Process(CAN_Data_TypeDef *encoder_data,short init_angle)
 *@Description:	���������ݴ��������������ֵ
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Encoder_Data_Process(CAN_Data_TypeDef *encoder_data,short init_angle)
{
	//init_angle������̨��ʼ�Ƕȵĵ��ԭʼֵ����Χ0-8191��
	//�жϳ�ʼ�Ƕ��Ƿ񳬹��������м�ֵ
	if(init_angle + 4096 >8191)
	{
		encoder_data->Critical_MechAngle = init_angle + 4096 - 8192;
	}
	else
	{
		encoder_data->Critical_MechAngle = init_angle + 4096;
	}
	
	//��ʼ�Ƕ�ȷ��
	encoder_data->Init_MechAngle = Angle_Correct_Get(init_angle,encoder_data->Critical_MechAngle);
}

/**
 *@Function:		Gimbal_Angle_Calibrate()
 *@Description:	��̨�Ƕȱ궨���ϵ�ʱ˫���ڰ˿���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Gimbal_Angle_Calibrate()
{
	if(DBUS.RC.Switch_Left == RC_SW_DOWN && DBUS.RC.Switch_Right == RC_SW_DOWN)
	{
	//�ڰ�״̬�½���궨����
	if(DBUS.RC.ch0 < -600 && DBUS.RC.ch1 <-600 && DBUS.RC.ch2 > 600 && DBUS.RC.ch3 < -600)
	{
		//������������
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(800,100);
		delay_ms(500);		
		Beep_Ctrl(800,0);
		//������������
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(800,100);
		delay_ms(500);		
		Beep_Ctrl(800,0);
		
		//�ȴ��󲦸�����
		while(DBUS.RC.Switch_Left != RC_SW_MID)
		{
			Angle_Init.Yaw_Angle_Init = CAN_Gimbal[0].Origin_MechAngle;
		}
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(1000,0);
		//�ȴ��󲦸�����
		while(DBUS.RC.Switch_Left != RC_SW_UP)
		{
			Angle_Init.Pitch_Angle_Init = CAN_Gimbal[1].Origin_MechAngle;
		}
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(1000,0);
		//�ȴ��Ҳ�������
		while(DBUS.RC.Switch_Right != RC_SW_MID)
		{
			Angle_Init.Pitch_Angle_Max = CAN_Gimbal[1].Origin_MechAngle;			
		}
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(1000,0);
		//�ȴ��Ҳ�������
		while(DBUS.RC.Switch_Right != RC_SW_UP)
		{
			Angle_Init.Pitch_Angle_Min = CAN_Gimbal[1].Origin_MechAngle;			
		}		
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(800,100);
		delay_ms(500);		
		Beep_Ctrl(800,0);
		
		STMFLASH_Write(CONFIG_PARAM_ADDR,(u32 *)&Angle_Init, sizeof(Angle_Init));	/*д��stm32 flash*/

	}		
	}
}


/**
 *@Function:	  Gimbal_Angle_Read()	
 *@Description:	��̨�Ƕ�ֵ��ȡ
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Gimbal_Angle_Read()
{
	//����սǶ�ֵ
	memset(&Angle_Init,0,sizeof(Angle_Init));
	//��ȡFlashֵ
	STMFLASH_Read(CONFIG_PARAM_ADDR, (u32 *)&Angle_Init, sizeof(Angle_Init));
	//�ж��Ƿ�Ϊ��Чֵ
	if(Angle_Init.Pitch_Angle_Init == 0 && Angle_Init.Pitch_Angle_Max == 0 && Angle_Init.Pitch_Angle_Min == 0 && Angle_Init.Yaw_Angle_Init == 0 )
	{
			Beep_Ctrl(1000,100);
			delay_ms(500);
			Beep_Ctrl(800,100);
			delay_ms(500);	
			Beep_Ctrl(1000,100);
			delay_ms(500);
			Beep_Ctrl(800,0);
			//�����ȴ�
			while(1){}
	}
}


short Angle_Last ;
short Init_Angle ;

short Origin_Init_Yaw_Angle;
short Origin_Init_Pitch_Angle;

extern float Pitch_Aim_Angle;
extern float Pitch_Angle_Max;
extern float Pitch_Angle_Min;
/**
 *@Function:		Gimbal_Init()
 *@Description:	��̨��ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Gimbal_Init()
{
		//��̨�Ƕȱ궨
		Gimbal_Angle_Calibrate();
	
	  //��̨PID��ʼ��
		Pid_Reset(&PID_Gimbal_Angle[0]);
		Pid_Reset(&PID_Gimbal_Angle[1]);
		Pid_Reset(&PID_Gimbal_Speed[0]);
		Pid_Reset(&PID_Gimbal_Speed[1]);
		
	//����i�ķ���
	
//		Pid_Set(&PID_Gimbal_Angle[0],22,0,0,10000,3000,30000,10000,1,30000,0,2);	//Yaw
//		Pid_Set(&PID_Gimbal_Speed[0],400,0,0,30000,5000,200,30000,1,30000,0,2);	  //Yaw	

//		Pid_Set(&PID_Gimbal_Angle[1],3,0,0,10000,0,30000,10000,1,30000,0,2);		  //Pitch
//		Pid_Set(&PID_Gimbal_Speed[1],90,2,0,30000,7000,30000,30000,1,30000,0,2);  //Pitch
	
		Pid_Set(&PID_Gimbal_Angle[0],25,0,0,10000,3000,30000,10000,1,30000,0,2);	//Yaw
		Pid_Set(&PID_Gimbal_Speed[0],370,0,0,30000,5000,200,30000,1,30000,0,2);	  //Yaw	

		Pid_Set(&PID_Gimbal_Angle[1],2,0,0,10000,0,30000,10000,1,30000,0,2);		  //Pitch
		Pid_Set(&PID_Gimbal_Speed[1],80,0,0,30000,7000,30000,30000,1,30000,0,2);  //Pitch
	
		//��ȡ������Flash����ֵ
		Gimbal_Angle_Read();
	
		//��̨��ʼֵ����
		Encoder_Data_Process(&CAN_Gimbal[0],Angle_Init.Yaw_Angle_Init);
		Encoder_Data_Process(&CAN_Gimbal[1],Angle_Init.Pitch_Angle_Init);
		
		//��̨��ʵ�Ƕȴ���
		Pitch_Aim_Angle = Angle_Correct_Get(Angle_Init.Pitch_Angle_Init,CAN_Gimbal[1].Critical_MechAngle);
		Pitch_Angle_Max = Angle_Correct_Get(Angle_Init.Pitch_Angle_Max,CAN_Gimbal[1].Critical_MechAngle);
		Pitch_Angle_Min = Angle_Correct_Get(Angle_Init.Pitch_Angle_Min,CAN_Gimbal[1].Critical_MechAngle);
		//Pitch�����Сֵ����
		if(Pitch_Angle_Max < Pitch_Angle_Min)
		{
			Pitch_Angle_Min = Pitch_Angle_Max;
			Pitch_Angle_Max = Angle_Correct_Get(Angle_Init.Pitch_Angle_Min,CAN_Gimbal[1].Critical_MechAngle);
		}
		//Yaw���ʼ�Ƕ�ֵ
		Angle_Last = Init_Angle = Angle_Correct_Get(Angle_Init.Yaw_Angle_Init,CAN_Gimbal[0].Critical_MechAngle);
		//����YAW��ʼ��
		Origin_Init_Yaw_Angle = Angle_Init.Yaw_Angle_Init;
		//Pitch��ʼ��
		Origin_Init_Pitch_Angle = Pitch_Aim_Angle;

	  //�ȴ�һ���CAN���ݽ��պ�ͬ���Ƕ�
		delay_ms(10);
}



