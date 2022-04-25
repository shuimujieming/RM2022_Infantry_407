#include "chassis_task.h"
void Chassis_Speed_Calc(float vx,float vy,float vw);
void Chassis_Follow_Control();
void Chassis_Stop_Control();
void Chassis_Rotate_Control();

void Encoder_Data_Process(CAN_Data_TypeDef *encoder_data,short init_angle);

#define ANGLE_TO_RAD 0.01745329251994329576923690768489f

#define IS_SUPER_OFF (DBUS.RC.ch4 > -600 && !(DBUS.PC.Keyboard & KEY_SHIFT))
TaskHandle_t CHASSIS_Task_Handler;
void chassis_task(void *p_arg);

Chassis_Speed_Typedef Chassis_Speed;
Chassis_Mode_Enum Chassis_Mode;
Chassis_Control_Speed_Typedef rc;
Chassis_Control_Speed_Typedef keyboard;	
/**
 *@Function:		chassis_task_create()
 *@Description:	�������񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void chassis_task_create()
{
		xTaskCreate((TaskFunction_t )chassis_task,          //������
							(const char*    )"chassis_task",          //��������
							(uint16_t       )CHASSIS_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )CHASSIS_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&CHASSIS_Task_Handler);  //������  
}


void Chassis_Power_Limit();
void Chassis_SuperCap_Control();
extern int SuperCap_Signal;
extern int power_relay;
int Super_Allow_Flag = 0;

int time_delay = 0;

/**
 *@Function:		chassis_task(void *p_arg)
 *@Description:	��������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void chassis_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(3);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	while(1)
	{
		switch(Chassis_Mode)
		{
			//����ģʽ
			case CHASSIS_MODE_FOLLOW:
				Chassis_Follow_Control();
				break;
			//����ģʽ
			case CHASSIS_MODE_ROTATE:
				Chassis_Rotate_Control();
				break;
			//����ģʽ
			case CHASSIS_MODE_STOP:
				Chassis_Stop_Control();
				break;
			//���ģʽ
			case CHASSIS_MODE_RUNE:
				Chassis_Stop_Control();
				break;
			default:
				break;
		}
		
		//�����ٶȽ���
		Chassis_Speed_Calc(Chassis_Speed.vx,Chassis_Speed.vy,Chassis_Speed.vw);
		
		//��װ�˵���
		if(SuperCap_Signal > 0)
		{
			//���п��Ƶ���
			if(SuperCap_Info.id == 0x211)
			{
				Chassis_SuperCap_Control();			
			}
			//���Ƴ�������
			else if(SuperCap_Info.id == 0x300)
			{

				//���ݵ���15v������������ģʽ
				if(SuperCap_Info.CapVot < 15.0)
				{
						Super_Allow_Flag = 0;				
				}
				//�رյ���ģʽ�����Ƶ��̹���
				if(IS_SUPER_OFF)
				{
					Super_Allow_Flag = 1;
					
					time_delay = 0;
					power_relay = 0;
					Chassis_Power_Limit();
				}
				//�ֶ�������������ģʽ���ҵ�ѹ������
				else if(Super_Allow_Flag)
				{
					time_delay ++;
					if(time_delay < 50)
					{
						Chassis_Power_Limit();										
					}
					
					power_relay = 1;
				}
				//�ֶ�������������ģʽ���ڵ�ѹ�����������£���ʱǿ��
				//�лص�������ģʽ��������Ҫ�ֶ��رճ�������ģʽ������ٴο���
				else
				{
					time_delay = 0;
					power_relay = 0;
					Chassis_Power_Limit();				
				}
			}
		}
		//δ������ݣ�ʹ�ò���ϵͳ����
		else
		{
			//��������ģʽ��δʹ�õ������������
			//�رյ���ģʽ��
			if(IS_SUPER_OFF)
			{
				Chassis_Power_Limit();
			}			
		}



		//���Ŀ�������ֵ
		for(int i = 0;i<4;i++)
		{
			CAN_Chassis[i].Target_Current = Pid_Calc(&PID_Chassis[i], CAN_Chassis[i].Current_Speed, Chassis_Speed.wheel_speed[i]);		
		}
		//CAN���ݸ��·���
		xEventGroupSetBits(EventGroupHandler,CHASSIS_SIGNAL);	//�����¼�����λ		
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}

/**
 *@Function:	���̹�������	
 *@Description:	
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Chassis_Power_Limit()
{
		//���̹��ʿ���
//		if(power_heat_data.chassis_power_buffer < 60)
//		{
//			for(int i =0;i<4;i++)
//			{
//				Chassis_Speed.wheel_speed[i] *= power_heat_data.chassis_power_buffer/60.0f;
//			}
//		}	
}



/**
 *@Function:	���̵��ݿ��Ʋ���
 *@Description:	
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Chassis_SuperCap_Control()
{
	static float   power_limit = 1.0; //��������ϵ��
	static uint8_t power_flag  = 0; 	//���ٱ�־
	
	//�رյ��ݿ���
	if(IS_SUPER_OFF)
	{
		power_flag = 0;		
		Super_Allow_Flag = 1;
	}
	//��������ģʽ
	else if(Super_Allow_Flag)
	{
		power_flag = 1;		
	}
	else
	{
		power_flag = 0;			
	}
	
	//�����ݵ�ѹ����16vǿ������
	if(SuperCap_Info.Low_Filter_Vot < 16.0)
	{
		Super_Allow_Flag = 0;
		power_flag = 0;
	}
	
	//����״̬
	if(power_flag == 0)
	{
		//���̹��ʿ���ϵ��
		power_limit = (SuperCap_Info.Low_Filter_Vot - 10.0) / (SuperCap_Info.InputVot - 10.0);
		//ϵ���޷�
		if(power_limit >= 1.0)
		{
			power_limit = 1.0;
		}
		else if(power_limit < 0)
		{
			power_limit = 0;
		}
		//�ٶ�����
		for(int i =0;i<4;i++)
		{
			Chassis_Speed.wheel_speed[i] *= power_limit;
		}
	}
	//����״̬���������ٶ����ޣ�Ҳ���Զ��ٶȽ���������
	else if(power_flag == 1)
	{
		//�޸�power_limit���ٶ����޽����������ο�Chassis_Speed_Calc(float vx,float vy,float vw)
		power_limit = 1;		
			for(int i =0;i<4;i++)
		{
			Chassis_Speed.wheel_speed[i] *= power_limit;
		}
	}
		
}

//��һ��ֵ�Ƕ�
extern short Angle_Last;
//��ʼ�Ƕ�
extern short Init_Angle;

//����Criticalֵ
int Encoder_Data_Cal(CAN_Data_TypeDef *encoder_data,short init_angle)
{
	if(init_angle + 4096 >8191)
	{
		return init_angle + 4096 - 8192;
	}
	else
	{
		return init_angle + 4096;
	}
}
//���ص�ǰ��ʵ�Ƕ�ֵ
int Angle_Correct(int angle_to_resize)
{	
		if(angle_to_resize < CAN_Gimbal[0].Critical_MechAngle)
		{
			return angle_to_resize + 8192;
		}
		else
		{
			return angle_to_resize;
		}	
}
extern short Origin_Init_Yaw_Angle;

//�ٶȷ����־λ
int Init_Dir = 1;
/**
 *@Function:		Angle_Reverse_Handle()
 *@Description:	�Ƕȷ�ת����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Angle_Reverse_Handle()
{
		/*
		ʵ�ָ���״̬ʱ��С��תʱ�䣬��ͷβ����
	*/

	//�Ƕ�����
	if(abs(CAN_Gimbal[0].Current_MechAngle - Angle_Last) > 4000)
	{
		
		if(CAN_Gimbal[0].Critical_MechAngle == Encoder_Data_Cal(&CAN_Gimbal[0],Origin_Init_Yaw_Angle) )
		{
			//�������Ϊ��ǰ��
			Encoder_Data_Process(&CAN_Gimbal[0],Origin_Init_Yaw_Angle+4096);
			if(CAN_Gimbal[0].Origin_MechAngle < CAN_Gimbal[0].Critical_MechAngle)
			{
				CAN_Gimbal[0].Current_MechAngle = CAN_Gimbal[0].Origin_MechAngle + 8192;
			}
			else
			{
				CAN_Gimbal[0].Current_MechAngle = CAN_Gimbal[0].Origin_MechAngle;
			}		
			
			Init_Angle = Angle_Correct(Origin_Init_Yaw_Angle+4096);
			
		}
		else
		{
			//�������Ϊ��ǰ��
			Encoder_Data_Process(&CAN_Gimbal[0],Origin_Init_Yaw_Angle);
			if(CAN_Gimbal[0].Origin_MechAngle < CAN_Gimbal[0].Critical_MechAngle)
			{
				CAN_Gimbal[0].Current_MechAngle = CAN_Gimbal[0].Origin_MechAngle + 8192;
			}
			else
			{
				CAN_Gimbal[0].Current_MechAngle = CAN_Gimbal[0].Origin_MechAngle;
			}
			
			Init_Angle = Angle_Correct(Origin_Init_Yaw_Angle);
		}
		
		Init_Dir =-Init_Dir;
	}
	
	Angle_Last = CAN_Gimbal[0].Current_MechAngle;
}
int Chassis_Rotate_Base_Speed = 0;

/**
 *@Function:		Chassis_Follow_Control()
 *@Description:	���̸������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Chassis_Follow_Control()
{
	Chassis_Rotate_Base_Speed = 0;
	//ս���ٶȷ�����ֵ
	Chassis_Speed.vx = rc.vx + keyboard.vx;
	Chassis_Speed.vy = rc.vy + keyboard.vy;
	Chassis_Speed.vw = Pid_Calc(&PID_Chassis_Omega,(CAN_Gimbal[0].Current_MechAngle - Init_Angle)/8192.0*360.0f, 0); //��̨Yaw����Խ�PID �����ת�ٶȷ���
	
	Angle_Reverse_Handle();
	
	Chassis_Speed.vx *= Init_Dir;
	Chassis_Speed.vy *= Init_Dir;

	//��ת�ٶ����������پ�ֹ������ϵ����
	if(fabs(Chassis_Speed.vw) < 200)
	Chassis_Speed.vw = 0;
}

extern short Origin_Init_Yaw_Angle;
/**
 *@Function:		Chassis_Rotate_Control()
 *@Description:	�������ݿ���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Chassis_Rotate_Control()
{
	Chassis_Rotate_Base_Speed = 5000;
	
	static double Rotate_Angle = 0;
	
	Rotate_Angle = (CAN_Gimbal[0].Current_MechAngle - Origin_Init_Yaw_Angle)/8192.0*360.0f;
	if(Rotate_Angle <= 0)
	{
		Rotate_Angle = Rotate_Angle + 360.0f;
	}
	
	Chassis_Speed.vx = +(rc.vy + keyboard.vy) * sin(Rotate_Angle * ANGLE_TO_RAD) + (rc.vx + keyboard.vx) * cos(Rotate_Angle * ANGLE_TO_RAD);
	Chassis_Speed.vy = +(rc.vy + keyboard.vy) * cos(Rotate_Angle * ANGLE_TO_RAD) - (rc.vx + keyboard.vx) * sin(Rotate_Angle * ANGLE_TO_RAD);
	Chassis_Speed.vw = Chassis_Rotate_Base_Speed;
	
	Angle_Reverse_Handle();
}
/**
 *@Function:		Chassis_Stop_Control()
 *@Description:	������������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Chassis_Stop_Control()
{
  Chassis_Rotate_Base_Speed = 0;

	Chassis_Speed.vx = 0;
	Chassis_Speed.vy = 0;
	Chassis_Speed.vw = 0;
}

/**
 *@Function:		Chassis_Speed_Calc(float vx,float vy,float vw)
 *@Description:	�����ٶȽ��� vxΪǰ���˶��ٶȣ�vyΪ����ƽ���˶��ٶȣ�vwΪ��ת�ٶ�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Chassis_Speed_Calc(float vx,float vy,float vw)
{
	float Speed_Max = 0;
	//���ٽ���
//	Chassis_Speed.wheel_speed[0] = +vx + vy + vw;
//	Chassis_Speed.wheel_speed[1] = -vx + vy + vw;
//	Chassis_Speed.wheel_speed[2] = -vx - vy + vw;
//	Chassis_Speed.wheel_speed[3] = +vx - vy + vw;
	
	
	//XYƽ���ʼ�ٶȼн� y����x
	Rudder_Data.XY_Angle_Origin = atan2f(vy,vx);//���ȣ�-PI -- PI��
	//Yaw����ƫ���
	Rudder_Data.Yaw_Angle_Offset = (Init_Angle - CAN_Gimbal[0].Current_MechAngle) / 8192.0f * 360.0f * ANGLE_TO_RAD;
	//XYƽ����ʵ�ٶȼн�
	Rudder_Data.XY_Angle_Real = Rudder_Data.XY_Angle_Origin + Rudder_Data.Yaw_Angle_Offset;
  //XYƽ���ٶȼ��㣬ƽ���Ϳ�����
	Rudder_Data.XY_Speed = sqrt(vx*vx + vy*vy);
	
  //����ģʽ��ÿ���������ĳ�ʼƫ������
	Rudder_Data.XYZ_Angle_A[0] = Chassis_Rotate_Base_Speed * sin(45*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[0] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * cos(45*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_A[1] = Chassis_Rotate_Base_Speed * sin(135*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[1] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * cos(135*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_A[2] = Chassis_Rotate_Base_Speed * sin(225*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[2] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * cos(225*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_A[3] = Chassis_Rotate_Base_Speed * sin(315*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[3] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * cos(315*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	


/*              �޾ͽ�ԭ����߼�                */
	for(int i =0;i<4;i++)
	{
    //���պϳɵ�XYZ������Լ�����
		Rudder_Data.XYZ_Angle[i] = Rudder_Data.XY_Angle_Real + atan2f(Rudder_Data.XYZ_Angle_A[i],Rudder_Data.XYZ_Angle_B[i]);	
		Rudder_Data.XYZ_Speed[i] = sqrt(Rudder_Data.XYZ_Angle_A[i]*Rudder_Data.XYZ_Angle_A[i] + Rudder_Data.XYZ_Angle_B[i]*Rudder_Data.XYZ_Angle_B[i]);
		
		//������Ƿ�Χ������+-180��
		if(Rudder_Data.XYZ_Angle[i] > 180.0*ANGLE_TO_RAD)
		{
			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XYZ_Angle[i] - 360.0*ANGLE_TO_RAD;
		}
		else if(Rudder_Data.XYZ_Angle[i] < -180.0*ANGLE_TO_RAD)
		{
			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XYZ_Angle[i] + 360.0*ANGLE_TO_RAD;
		}

		//��ǰ�����
		Rudder_Data.XYZ_Angle_Current[i] = (CAN_Rudder[i].Current_MechAngle - CAN_Rudder[i].Init_MechAngle) / 8191.0f * 360.0f * ANGLE_TO_RAD;
		
		//�Ƕ����䴦��+-180�ȱ߽���
		/*������������������������������������������������������������������������������������������*/
		//���統ǰ-170�㣬Ŀ��170��
		if(Rudder_Data.XYZ_Angle[i] - Rudder_Data.XYZ_Angle_Current[i] > 180.0 * ANGLE_TO_RAD)
		{
			//����Ŀ��Ϊ 170 - 360 = -190��
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i] - 360.0*ANGLE_TO_RAD;
		}
		//���統ǰ170�㣬Ŀ��-170��
		else if(Rudder_Data.XYZ_Angle[i] - Rudder_Data.XYZ_Angle_Current[i] < -180.0 * ANGLE_TO_RAD)
		{
			//����Ŀ��Ϊ-170 + 360 = 190��
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i] + 360.0*ANGLE_TO_RAD;			
		}
		else
		{
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i];
		}
		
		
    //�����PID����
		PID_Rudder_Angle[i].PIDout = Pid_Calc(&PID_Rudder_Angle[i],Rudder_Data.XYZ_Angle_Current[i],Rudder_Data.XYZ_Angle_Target[i]);
		CAN_Rudder[i].Target_Current = Pid_Calc(&PID_Rudder_Speed[i],CAN_Rudder[i].Current_Speed,PID_Rudder_Angle[i].PIDout);
		
		//��¼�ϴζ����
		Rudder_Data.XYZ_Angle_Last[i] = Rudder_Data.XYZ_Angle[i];
	}	
	
  //�������
	Chassis_Speed.wheel_speed[0] = Rudder_Data.XYZ_Speed[0];
	Chassis_Speed.wheel_speed[1] = -Rudder_Data.XYZ_Speed[1];
	Chassis_Speed.wheel_speed[2] = -Rudder_Data.XYZ_Speed[2];
	Chassis_Speed.wheel_speed[3] = Rudder_Data.XYZ_Speed[3];


//	/*              �оͽ�ԭ����߼�                */
//	for(int i =0;i<4;i++)
//	{
//    //���պϳɵ�XYZ������Լ�����
//		Rudder_Data.XYZ_Angle[i] = Rudder_Data.XY_Angle_Real + atan2f(Rudder_Data.XYZ_Angle_A[i],Rudder_Data.XYZ_Angle_B[i]);	
//		Rudder_Data.XYZ_Speed[i] = sqrt(Rudder_Data.XYZ_Angle_A[i]*Rudder_Data.XYZ_Angle_A[i] + Rudder_Data.XYZ_Angle_B[i]*Rudder_Data.XYZ_Angle_B[i]);
//		
//		//������Ƿ�Χ������+-180��
//		if(Rudder_Data.XYZ_Angle[i] > 180.0*ANGLE_TO_RAD)
//		{
//			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XYZ_Angle[i] - 360.0*ANGLE_TO_RAD;
//		}
//		else if(Rudder_Data.XYZ_Angle[i] < -180.0*ANGLE_TO_RAD)
//		{
//			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XYZ_Angle[i] + 360.0*ANGLE_TO_RAD;
//		}

//		//��ǰ�����
//		Rudder_Data.XYZ_Angle_Current[i] = (CAN_Rudder[i].Current_MechAngle - CAN_Rudder[i].Init_MechAngle) / 8191.0f * 360.0f * ANGLE_TO_RAD;
//		
//		//�Ƕ����䴦��+-180�ȱ߽���
//		/*������������������������������������������������������������������������������������������*/
//		//���統ǰ-170�㣬Ŀ��170��
//		if(Rudder_Data.XYZ_Angle[i] - Rudder_Data.XYZ_Angle_Current[i] > 180.0 * ANGLE_TO_RAD)
//		{
//			//����Ŀ��Ϊ 170 - 360 = -190��
//			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i] - 360.0*ANGLE_TO_RAD;
//		}
//		//���統ǰ170�㣬Ŀ��-170��
//		else if(Rudder_Data.XYZ_Angle[i] - Rudder_Data.XYZ_Angle_Current[i] < -180.0 * ANGLE_TO_RAD)
//		{
//			//����Ŀ��Ϊ-170 + 360 = 190��
//			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i] + 360.0*ANGLE_TO_RAD;			
//		}
//		else
//		{
//			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i];
//		}
//		
//		//���統ǰ170�㣬Ŀ��-20�㣬
//		//��ͨ��������������㣬�ó�Ŀ��Ϊ -20 + 360 = 340�� 
//		//�ͽ�ԭ��������Ӧ�õ���Ϊ160�㣬���ӷ���
//		//����340��- 170���ж��Ƿ����90��
//		//����Ŀ��Ϊ340 - 180 = 160�㣬Ȼ�����ӷ���
//		if(Rudder_Data.XYZ_Angle_Target[i] - Rudder_Data.XYZ_Angle_Current[i] > 90.0*ANGLE_TO_RAD)
//		{
//			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle_Target[i] - 180.0*ANGLE_TO_RAD;
//			Rudder_Data.XYZ_Speed_Dir = -1;
//		}
//		else if(Rudder_Data.XYZ_Angle_Target[i] - Rudder_Data.XYZ_Angle_Current[i] < -90.0*ANGLE_TO_RAD)
//		{
//			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle_Target[i] + 180.0*ANGLE_TO_RAD;			
//			Rudder_Data.XYZ_Speed_Dir = -1;
//		}
//		else
//		{
//			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle_Target[i];
//			Rudder_Data.XYZ_Speed_Dir = 1;
//		}
//		
//    //�����PID����
//		PID_Rudder_Angle[i].PIDout = Pid_Calc(&PID_Rudder_Angle[i],Rudder_Data.XYZ_Angle_Current[i],Rudder_Data.XYZ_Angle_Target[i]);
//		CAN_Rudder[i].Target_Current = Pid_Calc(&PID_Rudder_Speed[i],CAN_Rudder[i].Current_Speed,PID_Rudder_Angle[i].PIDout);
//		
//		//��¼�ϴζ����
//		Rudder_Data.XYZ_Angle_Last[i] = Rudder_Data.XYZ_Angle[i];
//	}	
	
//  //�������
//	Chassis_Speed.wheel_speed[0] = Rudder_Data.XYZ_Speed[0] * Rudder_Data.XYZ_Speed_Dir;
//	Chassis_Speed.wheel_speed[1] = -Rudder_Data.XYZ_Speed[1] * Rudder_Data.XYZ_Speed_Dir;
//	Chassis_Speed.wheel_speed[2] = -Rudder_Data.XYZ_Speed[2] * Rudder_Data.XYZ_Speed_Dir;
//	Chassis_Speed.wheel_speed[3] = Rudder_Data.XYZ_Speed[3] * Rudder_Data.XYZ_Speed_Dir;

//	
	//�ٶ��޷�����
	//���ֵѰ��
	for(int i = 0;i<4;i++)
	{
		if(abs(Chassis_Speed.wheel_speed[i]) > Speed_Max)
		{
			Speed_Max = abs(Chassis_Speed.wheel_speed[i]);
		}
	}
	//�����������
	if(Speed_Max > MAX_WHEEL_SPEED)
	{
		for(int i = 0;i<4;i++)
		{
			Chassis_Speed.wheel_speed[i] *= MAX_WHEEL_SPEED/Speed_Max;
		}
	}
	
}