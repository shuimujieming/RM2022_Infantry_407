#include "chassis_task.h"
void Chassis_Speed_Calc(float vx,float vy,float vw);
void Chassis_Follow_Control();
void Chassis_Stop_Control();
void Chassis_Rotate_Control();


void Chassis_Rudder_Nearby_Cal(float vx,float vy,float vw);
void Chassis_Rudder_NonNearby_Cal(float vx,float vy,float vw);


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
 *@Description:	底盘任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void chassis_task_create()
{
		xTaskCreate((TaskFunction_t )chassis_task,          //任务函数
							(const char*    )"chassis_task",          //任务名称
							(uint16_t       )CHASSIS_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )CHASSIS_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&CHASSIS_Task_Handler);  //任务句柄  
}


void Chassis_Power_Limit();
void Chassis_SuperCap_Control();
extern int SuperCap_Signal;
extern int power_relay;
int Super_Allow_Flag = 0;

int time_delay = 0;

/**
 *@Function:		chassis_task(void *p_arg)
 *@Description:	底盘任务
 *@Param:       形参
 *@Return:	  	返回值
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
			//跟随模式
			case CHASSIS_MODE_FOLLOW:
				Chassis_Follow_Control();
				break;
			//陀螺模式
			case CHASSIS_MODE_ROTATE:
				Chassis_Rotate_Control();
				break;
			//无力模式
			case CHASSIS_MODE_STOP:
				Chassis_Stop_Control();
				break;
			//打符模式
			case CHASSIS_MODE_RUNE:
				Chassis_Stop_Control();
				break;
			default:
				break;
		}
		
		//底盘速度解算
//		Chassis_Speed_Calc(Chassis_Speed.vx,Chassis_Speed.vy,Chassis_Speed.vw);
		//舵轮速度解算
//			Chassis_Rudder_NonNearby_Cal(Chassis_Speed.vx,Chassis_Speed.vy,Chassis_Speed.vw); //无就近原则
			Chassis_Rudder_Nearby_Cal(Chassis_Speed.vx,Chassis_Speed.vy,Chassis_Speed.vw); //就近原则
		
		//安装了电容
		if(SuperCap_Signal > 0)
		{
			//雾列控制电容
			if(SuperCap_Info.id == 0x211)
			{
				Chassis_SuperCap_Control();			
			}
			//自制超级电容
			else if(SuperCap_Info.id == 0x300)
			{

				//电容低于15v后不允许开启电容模式
				if(SuperCap_Info.CapVot < 15.0)
				{
						Super_Allow_Flag = 0;				
				}
				//关闭电容模式下限制底盘功率
				if(IS_SUPER_OFF)
				{
					Super_Allow_Flag = 1;
					
					time_delay = 0;
					power_relay = 0;
					Chassis_Power_Limit();
				}
				//手动开启超级电容模式，且电压允许下
				else if(Super_Allow_Flag)
				{
					time_delay ++;
					if(time_delay < 50)
					{
						Chassis_Power_Limit();										
					}
					
					power_relay = 1;
				}
				//手动开启超级电容模式，在电压不允许的情况下，此时强制
				//切回底盘限速模式，并且需要手动关闭超级电容模式后才能再次开启
				else
				{
					time_delay = 0;
					power_relay = 0;
					Chassis_Power_Limit();				
				}
			}
		}
		//未接入电容，使用裁判系统数据
		else
		{
			//开启超速模式，未使用电容情况下慎用
			//关闭电容模式下
			if(IS_SUPER_OFF)
			{
				Chassis_Power_Limit();
			}			
		}

		//电机目标电流赋值
		for(int i = 0;i<4;i++)
		{
			CAN_Chassis[i].Target_Current = Pid_Calc(&PID_Chassis[i], CAN_Chassis[i].Current_Speed, Chassis_Speed.wheel_speed[i]);		
		}
		//CAN数据更新发送
		xEventGroupSetBits(EventGroupHandler,CHASSIS_SIGNAL);	//底盘事件组置位		
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}

/**
 *@Function:	底盘功率限制	
 *@Description:	
 *@Param:       形参
 *@Return:	  	返回值
 */
void Chassis_Power_Limit()
{
//		//底盘功率控制
//		if(power_heat_data.chassis_power_buffer < 60)
//		{
//			for(int i =0;i<4;i++)
//			{
//				Chassis_Speed.wheel_speed[i] *= power_heat_data.chassis_power_buffer/60.0f;
//			}
//		}	
}



/**
 *@Function:	底盘电容控制策略
 *@Description:	
 *@Param:       形参
 *@Return:	  	返回值
 */
void Chassis_SuperCap_Control()
{
	static float   power_limit = 1.0; //功率限制系数
	static uint8_t power_flag  = 0; 	//超速标志
	
	//关闭电容开关
	if(IS_SUPER_OFF)
	{
		power_flag = 0;		
		Super_Allow_Flag = 1;
	}
	//开启超速模式
	else if(Super_Allow_Flag)
	{
		power_flag = 1;		
	}
	else
	{
		power_flag = 0;			
	}
	
	//若电容电压低于16v强制限速
	if(SuperCap_Info.Low_Filter_Vot < 16.0)
	{
		Super_Allow_Flag = 0;
		power_flag = 0;
	}
	
	//限速状态
	if(power_flag == 0)
	{
		//底盘功率控制系数
		power_limit = (SuperCap_Info.Low_Filter_Vot - 10.0) / (SuperCap_Info.InputVot - 10.0);
		//系数限幅
		if(power_limit >= 1.0)
		{
			power_limit = 1.0;
		}
		else if(power_limit < 0)
		{
			power_limit = 0;
		}
		//速度限制
		for(int i =0;i<4;i++)
		{
			Chassis_Speed.wheel_speed[i] *= power_limit;
		}
	}
	//超速状态，不限制速度上限（也可以对速度进行提升）
	else if(power_flag == 1)
	{
		//修改power_limit对速度上限进行提升，参考Chassis_Speed_Calc(float vx,float vy,float vw)
		power_limit = 1;		
			for(int i =0;i<4;i++)
		{
			Chassis_Speed.wheel_speed[i] *= power_limit;
		}
	}
		
}

//上一中值角度
extern short Angle_Last;
//初始角度
extern short Init_Angle;

//返回Critical值
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
//返回当前真实角度值
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

//速度方向标志位
int Init_Dir = 1;
/**
 *@Function:		Angle_Reverse_Handle()
 *@Description:	角度反转处理
 *@Param:       形参
 *@Return:	  	返回值
 */
void Angle_Reverse_Handle()
{
		/*
		实现跟随状态时最小旋转时间，即头尾合用
	*/

	//角度跳变
	if(abs(CAN_Gimbal[0].Current_MechAngle - Angle_Last) > 4000)
	{
		
		if(CAN_Gimbal[0].Critical_MechAngle == Encoder_Data_Cal(&CAN_Gimbal[0],Origin_Init_Yaw_Angle) )
		{
			//将反面改为正前方
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
			//将反面改为正前方
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
 *@Description:	底盘跟随控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Chassis_Follow_Control()
{
	Chassis_Rotate_Base_Speed = 0;
	//战车速度分量赋值
	Chassis_Speed.vx = rc.vx + keyboard.vx;
	Chassis_Speed.vy = rc.vy + keyboard.vy;
	Chassis_Speed.vw = Pid_Calc(&PID_Chassis_Omega,(CAN_Gimbal[0].Current_MechAngle - Init_Angle)/8192.0*360.0f, 0); //云台Yaw轴相对角PID 输出旋转速度分量
	
	Angle_Reverse_Handle();
	
	Chassis_Speed.vx *= Init_Dir;
	Chassis_Speed.vy *= Init_Dir;

	//旋转速度死区，减少静止底盘轮系抖动
	if(fabs(Chassis_Speed.vw) < 200)
	Chassis_Speed.vw = 0;
}

extern short Origin_Init_Yaw_Angle;
/**
 *@Function:		Chassis_Rotate_Control()
 *@Description:	底盘陀螺控制
 *@Param:       形参
 *@Return:	  	返回值
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
 *@Description:	底盘无力控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Chassis_Stop_Control()
{
  Chassis_Rotate_Base_Speed = 0;

	Chassis_Speed.vx = 0;
	Chassis_Speed.vy = 0;
	Chassis_Speed.vw = 0;
}

//轮速最大值限制
void Chassis_Max_Limit()
{
	float Speed_Max = 0;
	
	//速度限幅调整
	//最大值寻找
	for(int i = 0;i<4;i++)
	{
		if(abs(Chassis_Speed.wheel_speed[i]) > Speed_Max)
		{
			Speed_Max = abs(Chassis_Speed.wheel_speed[i]);
		}
	}
	//最大轮速限制
	if(Speed_Max > MAX_WHEEL_SPEED)
	{
		for(int i = 0;i<4;i++)
		{
			Chassis_Speed.wheel_speed[i] *= MAX_WHEEL_SPEED/Speed_Max;
		}
	}
}

//底盘舵轮无就近原则解算
void Chassis_Rudder_NonNearby_Cal(float vx,float vy,float vw)
{
	//XY平面初始速度夹角 y除以x
	//弧度（-PI -- PI）
	Rudder_Data.XY_Angle_Origin = atan2f(vy,vx);
	//Yaw轴电机偏差角
	//弧度（-PI -- PI）
	Rudder_Data.Yaw_Angle_Offset = (Init_Angle - CAN_Gimbal[0].Current_MechAngle) / 8192.0f * 360.0f * ANGLE_TO_RAD; 
	//XY平面真实速度夹角（原始XY速度夹角与Yaw轴角度偏差叠加）
	Rudder_Data.XY_Angle_Real = Rudder_Data.XY_Angle_Origin + Rudder_Data.Yaw_Angle_Offset;
  	//XY平面速度计算，平方和开根号
//	Rudder_Data.XY_Speed = sqrt(vx*vx + vy*vy);
	arm_sqrt_f32(vx*vx + vy*vy,&Rudder_Data.XY_Speed);
	
	
  	//陀螺模式
	//每个舵向电机的初始XY偏移向量与旋转向量的合成
	//此处的45度、135度、225度、315度是未经过就近原则处理的默认角度值
	//A,B方向向量，含速度大小与方向
	Rudder_Data.XYZ_Angle_A[0] = Chassis_Rotate_Base_Speed * arm_sin_f32(45*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[0] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * arm_cos_f32(45*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_A[1] = Chassis_Rotate_Base_Speed * arm_sin_f32(135*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[1] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * arm_cos_f32(135*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_A[2] = Chassis_Rotate_Base_Speed * arm_sin_f32(225*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[2] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * arm_cos_f32(225*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_A[3] = Chassis_Rotate_Base_Speed * arm_sin_f32(315*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[3] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * arm_cos_f32(315*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	


/*              无就近原则的逻辑                */
	for(int i =0;i<4;i++)
	{
    	//最终合成的XYZ舵向角以及轮速
		//很关键，重点的地方，角度加上了XY_Angle_Real，加上了初始的偏移角。合成初始偏移角与AB合成角为真实角度方向
		Rudder_Data.XYZ_Angle[i] = Rudder_Data.XY_Angle_Real + atan2f(Rudder_Data.XYZ_Angle_A[i],Rudder_Data.XYZ_Angle_B[i]);	
		//速度大小仅由AB合成速度决定
//		Rudder_Data.XYZ_Speed[i] = sqrt(Rudder_Data.XYZ_Angle_A[i]*Rudder_Data.XYZ_Angle_A[i] + Rudder_Data.XYZ_Angle_B[i]*Rudder_Data.XYZ_Angle_B[i]);
		arm_sqrt_f32((Rudder_Data.XYZ_Angle_A[i]*Rudder_Data.XYZ_Angle_A[i] + Rudder_Data.XYZ_Angle_B[i]*Rudder_Data.XYZ_Angle_B[i]),&Rudder_Data.XYZ_Speed[i]);
		
		//将舵向角范围调整至+-180度
		if(Rudder_Data.XYZ_Angle[i] > 180.0*ANGLE_TO_RAD)
		{
			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XYZ_Angle[i] - 360.0*ANGLE_TO_RAD;
		}
		else if(Rudder_Data.XYZ_Angle[i] < -180.0*ANGLE_TO_RAD)
		{
			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XYZ_Angle[i] + 360.0*ANGLE_TO_RAD;
		}

		//当前舵向角（-PI -- PI）
		Rudder_Data.XYZ_Angle_Current[i] = (CAN_Rudder[i].Current_MechAngle - CAN_Rudder[i].Init_MechAngle) / 8191.0f * 360.0f * ANGLE_TO_RAD;
		
		//角度跳变处理，在+-180度边界上
		/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
		/*
		        0
			  /   \
		  -180—————180	
		*/
		//下一时刻目标角度与当前角度差值大于180度或小于-180度，即角度跳变处
		//假如当前-170°，目标170°
		if(Rudder_Data.XYZ_Angle[i] - Rudder_Data.XYZ_Angle_Current[i] > 180.0 * ANGLE_TO_RAD)
		{
			//则设目标为 170 - 360 = -190°
			//为了舵向电机旋转方向正确，走最快方向，越界之后当前角就变回正常值了，如175度，则目标170度就正常处理了
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i] - 360.0*ANGLE_TO_RAD;
		}
		//假如当前170°，目标-170°
		else if(Rudder_Data.XYZ_Angle[i] - Rudder_Data.XYZ_Angle_Current[i] < -180.0 * ANGLE_TO_RAD)
		{
			//则设目标为-170 + 360 = 190°
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i] + 360.0*ANGLE_TO_RAD;			
		}
		//无跳变则不需要处理
		else
		{
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i];
		}
		
		
    	//舵向角PID计算
		PID_Rudder_Angle[i].PIDout = Pid_Calc(&PID_Rudder_Angle[i],Rudder_Data.XYZ_Angle_Current[i],Rudder_Data.XYZ_Angle_Target[i]);
		CAN_Rudder[i].Target_Current = Pid_Calc(&PID_Rudder_Speed[i],CAN_Rudder[i].Current_Speed,PID_Rudder_Angle[i].PIDout);
		
		//记录上次舵向角（未用）
		Rudder_Data.XYZ_Angle_Last[i] = Rudder_Data.XYZ_Angle[i];
	}	
	
  	//轮速输出（注意到左右轮速相反，相同一侧速度同向）
	/*
	  对应电机ID 0x201-204
	  0——1
 	  3——2
	*/
	Chassis_Speed.wheel_speed[0] = Rudder_Data.XYZ_Speed[0];
	Chassis_Speed.wheel_speed[1] = -Rudder_Data.XYZ_Speed[1];
	Chassis_Speed.wheel_speed[2] = -Rudder_Data.XYZ_Speed[2];
	Chassis_Speed.wheel_speed[3] = Rudder_Data.XYZ_Speed[3];	
	
	//最大轮速限制
	Chassis_Max_Limit();
}
short Yaw_Test = 0;

//底盘舵轮就近原则解算
void Chassis_Rudder_Nearby_Cal(float vx,float vy,float vw)
{
	
	Yaw_Test += DBUS.RC.ch2 / 660.0f * 20.0f;
	
	if(Yaw_Test < 0)
	{
		Yaw_Test += 8191;
	}
	else if(Yaw_Test > 8191)
	{
		Yaw_Test -=8191;
	}
	
	Chassis_Rotate_Base_Speed = ((-DBUS.RC.ch2) / 660.0 + (-DBUS.PC.X / 100.0)) * 3000;
	
		//XY平面初始速度夹角 y除以x
	//弧度（-PI -- PI）
	Rudder_Data.XY_Angle_Origin = atan2f(vy,vx);
	//Yaw轴电机偏差角
	//弧度（-PI -- PI）
//	Rudder_Data.Yaw_Angle_Offset = (Init_Angle - CAN_Gimbal[0].Current_MechAngle) / 8192.0f * 360.0f * ANGLE_TO_RAD; 
	
	Rudder_Data.Yaw_Angle_Offset = (0) / 8192.0f * 360.0f * ANGLE_TO_RAD; 

	
	
	//XY平面真实速度夹角（原始XY速度夹角与Yaw轴角度偏差叠加）
	Rudder_Data.XY_Angle_Real = Rudder_Data.XY_Angle_Origin + Rudder_Data.Yaw_Angle_Offset;
  	//XY平面速度计算，平方和开根号
	arm_sqrt_f32(vx*vx + vy*vy,&Rudder_Data.XY_Speed);
	
	
  	//陀螺模式
	//每个舵向电机的初始XY偏移向量与旋转向量的合成
	//此处的45度、135度、225度、315度是未经过就近原则处理的默认角度值
	//A,B方向向量，含速度大小与方向
	Rudder_Data.XYZ_Angle_A[0] = Chassis_Rotate_Base_Speed * arm_sin_f32(45*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[0] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * arm_cos_f32(45*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_A[1] = Chassis_Rotate_Base_Speed * arm_sin_f32(135*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[1] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * arm_cos_f32(135*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_A[2] = Chassis_Rotate_Base_Speed * arm_sin_f32(225*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[2] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * arm_cos_f32(225*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_A[3] = Chassis_Rotate_Base_Speed * arm_sin_f32(315*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	Rudder_Data.XYZ_Angle_B[3] = Rudder_Data.XY_Speed + Chassis_Rotate_Base_Speed * arm_cos_f32(315*ANGLE_TO_RAD - Rudder_Data.XY_Angle_Real);
	
	
	
	
	/*              有就近原则的逻辑                */
	for(int i =0;i<4;i++)
	{
    //最终合成的XYZ舵向角以及轮速
		if(Rudder_Data.XYZ_Angle_B[i] == 0)
		{
			if(i%2 == 0)
			{
			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XY_Angle_Real + (1);																
			}	
			else
				{
			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XY_Angle_Real + (-1);												
				}
		}
		else
		{
			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XY_Angle_Real + atan2f(Rudder_Data.XYZ_Angle_A[i],Rudder_Data.XYZ_Angle_B[i]);				
		}
		arm_sqrt_f32((Rudder_Data.XYZ_Angle_A[i]*Rudder_Data.XYZ_Angle_A[i] + Rudder_Data.XYZ_Angle_B[i]*Rudder_Data.XYZ_Angle_B[i]),&Rudder_Data.XYZ_Speed[i]);
		
		//将舵向角范围调整至+-180度
		if(Rudder_Data.XYZ_Angle[i] > 180.0*ANGLE_TO_RAD)
		{
			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XYZ_Angle[i] - 360.0*ANGLE_TO_RAD;
		}
		else if(Rudder_Data.XYZ_Angle[i] < -180.0*ANGLE_TO_RAD)
		{
			Rudder_Data.XYZ_Angle[i] = Rudder_Data.XYZ_Angle[i] + 360.0*ANGLE_TO_RAD;
		}

		//当前舵向角
		Rudder_Data.XYZ_Angle_Current[i] = (CAN_Rudder[i].Current_MechAngle - CAN_Rudder[i].Init_MechAngle) / 8191.0f * 360.0f * ANGLE_TO_RAD;
		
		//角度跳变处理，+-180度边界上
		/*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
		//假如当前-170°，目标170°
		if(Rudder_Data.XYZ_Angle[i] - Rudder_Data.XYZ_Angle_Current[i] >= 180.0 * ANGLE_TO_RAD)
		{
			//则设目标为 170 - 360 = -190°
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i] - 360.0*ANGLE_TO_RAD;
		}
		//假如当前170°，目标-170°
		else if(Rudder_Data.XYZ_Angle[i] - Rudder_Data.XYZ_Angle_Current[i] <= -180.0 * ANGLE_TO_RAD)
		{
			//则设目标为-170 + 360 = 190°
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i] + 360.0*ANGLE_TO_RAD;			
		}
		else
		{
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle[i];
		}
		
		//假如当前170°，目标-20°，
		//先通过上面的跳变运算，得出目标为 -20 + 360 = 340° 
		//就近原则下我则应该调整为160°，轮子反向
		//则由340°- 170°判断是否大于90°
		//调整目标为340 - 180 = 160°，然后轮子反向
    //此部分处理仅需要判断目标角度是否是经过调整后的新角度，如果是则需要反向，不是则保持原方向
		if(Rudder_Data.XYZ_Angle_Target[i] - Rudder_Data.XYZ_Angle_Current[i] > 90.0*ANGLE_TO_RAD)
		{
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle_Target[i] - 180.0*ANGLE_TO_RAD;
			Rudder_Data.XYZ_Speed_Dir[i] = -1;
		}
		else if(Rudder_Data.XYZ_Angle_Target[i] - Rudder_Data.XYZ_Angle_Current[i] < -90.0*ANGLE_TO_RAD)
		{
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle_Target[i] + 180.0*ANGLE_TO_RAD;			
			Rudder_Data.XYZ_Speed_Dir[i] = -1;
		}
		else
		{
			Rudder_Data.XYZ_Angle_Target[i] = Rudder_Data.XYZ_Angle_Target[i];
			Rudder_Data.XYZ_Speed_Dir[i] = 1;
		}
		
    //舵向角PID计算
		PID_Rudder_Angle[i].PIDout = Pid_Calc(&PID_Rudder_Angle[i],Rudder_Data.XYZ_Angle_Current[i],Rudder_Data.XYZ_Angle_Target[i]);
		CAN_Rudder[i].Target_Current = Pid_Calc(&PID_Rudder_Speed[i],CAN_Rudder[i].Current_Speed,PID_Rudder_Angle[i].PIDout);
		
		//记录上次舵向角
		Rudder_Data.XYZ_Angle_Last[i] = Rudder_Data.XYZ_Angle[i];
	}	
	
  //轮速输出（含就近原则，故轮子有反转）
	Chassis_Speed.wheel_speed[0] = -Rudder_Data.XYZ_Speed[0] * Rudder_Data.XYZ_Speed_Dir[0];
	Chassis_Speed.wheel_speed[1] = Rudder_Data.XYZ_Speed[1] * Rudder_Data.XYZ_Speed_Dir[1];
	Chassis_Speed.wheel_speed[2] = Rudder_Data.XYZ_Speed[2] * Rudder_Data.XYZ_Speed_Dir[2];
	Chassis_Speed.wheel_speed[3] = -Rudder_Data.XYZ_Speed[3] * Rudder_Data.XYZ_Speed_Dir[3];

	//最大轮速限制
	Chassis_Max_Limit();
}

/**
 *@Function:		Chassis_Speed_Calc(float vx,float vy,float vw)
 *@Description:	底盘速度解算 vx为前后运动速度，vy为左右平移运动速度，vw为旋转速度
 *@Param:       形参
 *@Return:	  	返回值
 */
void Chassis_Speed_Calc(float vx,float vy,float vw)
{
  //轮速解算
	Chassis_Speed.wheel_speed[0] = +vx + vy + vw;
	Chassis_Speed.wheel_speed[1] = -vx + vy + vw;
	Chassis_Speed.wheel_speed[2] = -vx - vy + vw;
	Chassis_Speed.wheel_speed[3] = +vx - vy + vw;
	
	//最大轮速限制
	Chassis_Max_Limit();
}