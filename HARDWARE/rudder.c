#include "rudder.h"

extern void Encoder_Data_Process(CAN_Data_TypeDef *encoder_data,short init_angle);

Rudder_TypeDef Rudder_Data;

//舵初始化
void Rudder_Init()
{
	//舵轮组PID
	for(int i = 0;i<4;i++)
	{	
		Pid_Reset(&PID_Rudder_Angle[i]);
		Pid_Set(&PID_Rudder_Angle[i],400.0f,0.0f,0,30000,500,5000,30000,1,5000,0,0);
		Pid_Reset(&PID_Rudder_Speed[i]);
		Pid_Set(&PID_Rudder_Speed[i],210.0f,0.0f,0,30000,500,5000,30000,1,5000,0,0);
	}
	
	//舵的初始角度标定
	Encoder_Data_Process(&CAN_Rudder[0],4740);
	Encoder_Data_Process(&CAN_Rudder[1],3450);
	Encoder_Data_Process(&CAN_Rudder[2],6098);
	Encoder_Data_Process(&CAN_Rudder[3],6150);
}