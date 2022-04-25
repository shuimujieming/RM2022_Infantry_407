#include "main.h"

/*********************************************
Copyright(C) ˮľ��Ming
All rights reserved
File name:	main.c
Author:		shuimujieming
Version:		1.0
Description:	���������������е����г�ʼ��������ʹ��
Others:		
Log:	
*********************************************/

/*
*@Description���ײ��豸��ʼ��
*@param 1��	  ����1
*@param 2��	  ����2
*@return:��	  ����ֵ
*/
void BSP_Init()
{
	LED_Init();		        //��ʼ��LED�˿�

	Laser_Init();					//�����ʼ��
	
	DR16_Init();					//ң�������ջ���ʼ��
	
	CAN12_Init();					//CAN������ʼ��
	
	BMI088_init();				//BMI088��ʼ���������ǣ�

	TIM6_Init(1000);			//��ʱ��6��ʼ��
	
	Beep_Init();					//��������ʼ��
	
	Cover_Init();					//���ָǳ�ʼ��
	
	NUC_Init();						//NUC������ʼ��
	
	Referee_Init();				//����ϵͳ��ʼ��

	Gimbal_Init();				//��̨��ʼ��

	Chassis_Init();				//���̲�����ʼ��	
	
	Shoot_Init();					//�����ʼ��
	
	Rudder_Init();        //���ʼ��
	
}

/*
*@Description��������
*@param 1��	  ����1
*@param 2��	  ����2
*@return:��	  ����ֵ
*/
int main(void)
{ 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//�����ж����ȼ�����Ϊ4
	
	delay_init(168);		  //��ʼ����ʱ����
	
	BSP_Init();						//�ײ�����������ʼ��
	
	Task_Init();					//����ϵͳ�����ʼ��
		
}





