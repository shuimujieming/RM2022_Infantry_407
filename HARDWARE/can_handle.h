#ifndef __CAN_HANDLE_H
#define __CAN_HANDLE_H
#include "main.h"
/*********************************************
Copyright(C) 水木皆Ming
All rights reserved
File name:	can_handle.h
Author:		shuimujieming
Version:		1.0
Description:	CAN数据处理接收发送相关结构体定义
Others:		
Log:	
*********************************************/

typedef struct	
{
	short Current_Speed;
	short Current_MechAngle;
	short Current;
	short Temperature;
	short Critical_MechAngle;
	short Init_MechAngle;
	short Origin_MechAngle;
	short Target_Current;
	short Target_MechAngle;
	short Last_MechAngle;
	int Total_MechAngle;
}CAN_Data_TypeDef;


typedef struct	
{
	float InputVot;
	float CapVot;
	float InputCurrent;
	float CurrentPower;
	float Low_Filter_Vot;
	int id;
}SuperCap_TypeDef;

extern CAN_Data_TypeDef CAN_Chassis[4];
extern CAN_Data_TypeDef CAN_Gimbal[2];	//YAW 205 PITCH 209
extern CAN_Data_TypeDef CAN_Shoot[2];	//Trigger 0x206 Left_Fric 0x207 Right_Fric 0x208
extern CAN_Data_TypeDef CAN_Trigger;
extern SuperCap_TypeDef SuperCap_Info;
extern CAN_Data_TypeDef CAN_Rudder[4]; //0x205、0x209、0x20A、0x20B

void CAN1_TX_Chassis(void);
void CAN2_TX_Shoot(void);
void CAN1_TX_Gimbal_Pitch(void);
void CAN1_TX_Gimbal_Yaw(void);
void CAN1_TX_SuperCap(void);
void CAN2_TX_Rudder(void);

#endif