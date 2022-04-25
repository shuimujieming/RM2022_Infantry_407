#include "main.h"

/*********************************************
Copyright(C) 水木皆Ming
All rights reserved
File name:	main.c
Author:		shuimujieming
Version:		1.0
Description:	包含整个程序运行的所有初始化函数的使用
Others:		
Log:	
*********************************************/

/*
*@Description：底层设备初始化
*@param 1：	  参数1
*@param 2：	  参数2
*@return:：	  返回值
*/
void BSP_Init()
{
	LED_Init();		        //初始化LED端口

	Laser_Init();					//激光初始化
	
	DR16_Init();					//遥控器接收机初始化
	
	CAN12_Init();					//CAN驱动初始化
	
	BMI088_init();				//BMI088初始化（陀螺仪）

	TIM6_Init(1000);			//定时器6初始化
	
	Beep_Init();					//蜂鸣器初始化
	
	Cover_Init();					//弹仓盖初始化
	
	NUC_Init();						//NUC参数初始化
	
	Referee_Init();				//裁判系统初始化

	Gimbal_Init();				//云台初始化

	Chassis_Init();				//底盘参数初始化	
	
	Shoot_Init();					//发射初始化
	
	Rudder_Init();        //舵初始化
	
}

/*
*@Description：主函数
*@param 1：	  参数1
*@param 2：	  参数2
*@return:：	  返回值
*/
int main(void)
{ 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//设置中断优先级分组为4
	
	delay_init(168);		  //初始化延时函数
	
	BSP_Init();						//底层外设驱动初始化
	
	Task_Init();					//操作系统任务初始化
		
}





