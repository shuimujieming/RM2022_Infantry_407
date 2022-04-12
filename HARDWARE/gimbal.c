#include "gimbal.h"

//Flash信息储存地址
#define CONFIG_PARAM_SIZE	(1020*1024)
#define CONFIG_PARAM_ADDR 	(FLASH_BASE + CONFIG_PARAM_SIZE)	

Init_Angle_Data Angle_Init;

/**
 *@Function:		Angle_Correct_Get(int angle_to_resize , int angle_critical)
 *@Description:	角度纠正值获取，返回当前Critical值处理后的真实角度值
 *@Param:       形参
 *@Return:	  	返回值
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
 *@Description:	编码器数据处理，处理编码器中值
 *@Param:       形参
 *@Return:	  	返回值
 */
void Encoder_Data_Process(CAN_Data_TypeDef *encoder_data,short init_angle)
{
	//init_angle代表云台初始角度的电机原始值（范围0-8191）
	//判断初始角度是否超过编码器中间值
	if(init_angle + 4096 >8191)
	{
		encoder_data->Critical_MechAngle = init_angle + 4096 - 8192;
	}
	else
	{
		encoder_data->Critical_MechAngle = init_angle + 4096;
	}
	
	//初始角度确定
	encoder_data->Init_MechAngle = Angle_Correct_Get(init_angle,encoder_data->Critical_MechAngle);
}

/**
 *@Function:		Gimbal_Angle_Calibrate()
 *@Description:	云台角度标定，上电时双下内八开启
 *@Param:       形参
 *@Return:	  	返回值
 */
void Gimbal_Angle_Calibrate()
{
	if(DBUS.RC.Switch_Left == RC_SW_DOWN && DBUS.RC.Switch_Right == RC_SW_DOWN)
	{
	//内八状态下进入标定程序
	if(DBUS.RC.ch0 < -600 && DBUS.RC.ch1 <-600 && DBUS.RC.ch2 > 600 && DBUS.RC.ch3 < -600)
	{
		//蜂鸣器响两声
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(800,100);
		delay_ms(500);		
		Beep_Ctrl(800,0);
		//蜂鸣器响两声
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(800,100);
		delay_ms(500);		
		Beep_Ctrl(800,0);
		
		//等待左拨杆置中
		while(DBUS.RC.Switch_Left != RC_SW_MID)
		{
			Angle_Init.Yaw_Angle_Init = CAN_Gimbal[0].Origin_MechAngle;
		}
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(1000,0);
		//等待左拨杆置上
		while(DBUS.RC.Switch_Left != RC_SW_UP)
		{
			Angle_Init.Pitch_Angle_Init = CAN_Gimbal[1].Origin_MechAngle;
		}
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(1000,0);
		//等待右拨杆置中
		while(DBUS.RC.Switch_Right != RC_SW_MID)
		{
			Angle_Init.Pitch_Angle_Max = CAN_Gimbal[1].Origin_MechAngle;			
		}
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(1000,0);
		//等待右拨杆置上
		while(DBUS.RC.Switch_Right != RC_SW_UP)
		{
			Angle_Init.Pitch_Angle_Min = CAN_Gimbal[1].Origin_MechAngle;			
		}		
		Beep_Ctrl(1000,100);
		delay_ms(500);
		Beep_Ctrl(800,100);
		delay_ms(500);		
		Beep_Ctrl(800,0);
		
		STMFLASH_Write(CONFIG_PARAM_ADDR,(u32 *)&Angle_Init, sizeof(Angle_Init));	/*写入stm32 flash*/

	}		
	}
}


/**
 *@Function:	  Gimbal_Angle_Read()	
 *@Description:	云台角度值读取
 *@Param:       形参
 *@Return:	  	返回值
 */
void Gimbal_Angle_Read()
{
	//先清空角度值
	memset(&Angle_Init,0,sizeof(Angle_Init));
	//读取Flash值
	STMFLASH_Read(CONFIG_PARAM_ADDR, (u32 *)&Angle_Init, sizeof(Angle_Init));
	//判断是否为有效值
	if(Angle_Init.Pitch_Angle_Init == 0 && Angle_Init.Pitch_Angle_Max == 0 && Angle_Init.Pitch_Angle_Min == 0 && Angle_Init.Yaw_Angle_Init == 0 )
	{
			Beep_Ctrl(1000,100);
			delay_ms(500);
			Beep_Ctrl(800,100);
			delay_ms(500);	
			Beep_Ctrl(1000,100);
			delay_ms(500);
			Beep_Ctrl(800,0);
			//卡死等待
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
 *@Description:	云台初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void Gimbal_Init()
{
		//云台角度标定
		Gimbal_Angle_Calibrate();
	
	  //云台PID初始化
		Pid_Reset(&PID_Gimbal_Angle[0]);
		Pid_Reset(&PID_Gimbal_Angle[1]);
		Pid_Reset(&PID_Gimbal_Speed[0]);
		Pid_Reset(&PID_Gimbal_Speed[1]);
		
	//加了i的方案
	
//		Pid_Set(&PID_Gimbal_Angle[0],22,0,0,10000,3000,30000,10000,1,30000,0,2);	//Yaw
//		Pid_Set(&PID_Gimbal_Speed[0],400,0,0,30000,5000,200,30000,1,30000,0,2);	  //Yaw	

//		Pid_Set(&PID_Gimbal_Angle[1],3,0,0,10000,0,30000,10000,1,30000,0,2);		  //Pitch
//		Pid_Set(&PID_Gimbal_Speed[1],90,2,0,30000,7000,30000,30000,1,30000,0,2);  //Pitch
	
		Pid_Set(&PID_Gimbal_Angle[0],25,0,0,10000,3000,30000,10000,1,30000,0,2);	//Yaw
		Pid_Set(&PID_Gimbal_Speed[0],370,0,0,30000,5000,200,30000,1,30000,0,2);	  //Yaw	

		Pid_Set(&PID_Gimbal_Angle[1],2,0,0,10000,0,30000,10000,1,30000,0,2);		  //Pitch
		Pid_Set(&PID_Gimbal_Speed[1],80,0,0,30000,7000,30000,30000,1,30000,0,2);  //Pitch
	
		//读取机器人Flash参数值
		Gimbal_Angle_Read();
	
		//云台初始值解码
		Encoder_Data_Process(&CAN_Gimbal[0],Angle_Init.Yaw_Angle_Init);
		Encoder_Data_Process(&CAN_Gimbal[1],Angle_Init.Pitch_Angle_Init);
		
		//云台真实角度处理
		Pitch_Aim_Angle = Angle_Correct_Get(Angle_Init.Pitch_Angle_Init,CAN_Gimbal[1].Critical_MechAngle);
		Pitch_Angle_Max = Angle_Correct_Get(Angle_Init.Pitch_Angle_Max,CAN_Gimbal[1].Critical_MechAngle);
		Pitch_Angle_Min = Angle_Correct_Get(Angle_Init.Pitch_Angle_Min,CAN_Gimbal[1].Critical_MechAngle);
		//Pitch最大最小值处理
		if(Pitch_Angle_Max < Pitch_Angle_Min)
		{
			Pitch_Angle_Min = Pitch_Angle_Max;
			Pitch_Angle_Max = Angle_Correct_Get(Angle_Init.Pitch_Angle_Min,CAN_Gimbal[1].Critical_MechAngle);
		}
		//Yaw轴初始角度值
		Angle_Last = Init_Angle = Angle_Correct_Get(Angle_Init.Yaw_Angle_Init,CAN_Gimbal[0].Critical_MechAngle);
		//底盘YAW初始角
		Origin_Init_Yaw_Angle = Angle_Init.Yaw_Angle_Init;
		//Pitch初始角
		Origin_Init_Pitch_Angle = Pitch_Aim_Angle;

	  //等待一会儿CAN数据接收后同步角度
		delay_ms(10);
}



