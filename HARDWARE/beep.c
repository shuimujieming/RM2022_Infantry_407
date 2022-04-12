#include "beep.h"
/*********************************************
Copyright(C) ˮľ��Ming
All rights reserved
File name:	beep.c
Author:		shuimujieming
Version:		1.0
Description:	������ʹ��
Others:		
Log:	
*********************************************/
//������PWMʱ���Լ�GPIOʱ�ӿ���
#define RCC_APBPeriphClockCmd_TIM_BEEP RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE)
#define RCC_AHBPeriphClockCmd_GPIO_BEEP RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
//��������Ӧ��������
#define GPIO_BEEP GPIOD
#define GPIO_PinSource_BEEP GPIO_PinSource14
#define GPIO_AF_TIM_BEEP GPIO_AF_TIM4
#define GPIO_Pin_BEEP GPIO_Pin_14
//������PWMʱ��TIM�Լ���Ƶϵ��
#define TIM_BEEP TIM4
#define TIM_Psc_BEEP 84
//�Ƿ�Ϊ�߼���ʱ��
#define TIM_IS_ADVANCED_BEEP 0
//������PWM�����ʼ������Ӧ���PWM��ͨ���޸�
#define TIM_OCInit_BEEP		TIM_OC3Init(TIM_BEEP, &TIM_OCInitStructure);TIM_OC3PreloadConfig(TIM_BEEP, TIM_OCPreload_Enable)
//PWM����Ƚ�ͨ���޸�
#define BEEP_SOUND  TIM_BEEP->CCR3
//�������ӿڶ�Ӧʱ��
#define BEEP_TUNE   TIM_BEEP->ARR

/**
 *@Function:		Beep_Init()
 *@Description:	��������ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Beep_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APBPeriphClockCmd_TIM_BEEP;  
  RCC_AHBPeriphClockCmd_GPIO_BEEP;
	GPIO_PinAFConfig(GPIO_BEEP,GPIO_PinSource_BEEP,GPIO_AF_TIM_BEEP);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_BEEP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIO_BEEP, &GPIO_InitStructure); 
	
	TIM_TimeBaseStructure.TIM_Prescaler=TIM_Psc_BEEP-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=600-1;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM_BEEP,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	
	TIM_OCInit_BEEP;
	
  TIM_ARRPreloadConfig(TIM_BEEP,ENABLE);
	
	#if TIM_IS_ADVANCED_BEEP								//�Ƿ�Ϊ�߼���ʱ����������Ҫ�������PWM���ʹ��
	TIM_CtrlPWMOutputs(TIM_BEEP,ENABLE);
	#endif	
	
	TIM_Cmd(TIM_BEEP, ENABLE);
}

/**
 *@Function:		Beep_Ctrl(uint16_t tune, uint16_t sound)	
 *@Description:	����������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Beep_Ctrl(uint16_t tune, uint16_t sound)	
{
  BEEP_TUNE = tune;
  BEEP_SOUND = sound;
}
