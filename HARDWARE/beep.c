#include "beep.h"
/*********************************************
Copyright(C) 水木皆Ming
All rights reserved
File name:	beep.c
Author:		shuimujieming
Version:		1.0
Description:	蜂鸣器使用
Others:		
Log:	
*********************************************/
//蜂鸣器PWM时钟以及GPIO时钟开启
#define RCC_APBPeriphClockCmd_TIM_BEEP RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE)
#define RCC_AHBPeriphClockCmd_GPIO_BEEP RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
//蜂鸣器对应引脚配置
#define GPIO_BEEP GPIOD
#define GPIO_PinSource_BEEP GPIO_PinSource14
#define GPIO_AF_TIM_BEEP GPIO_AF_TIM4
#define GPIO_Pin_BEEP GPIO_Pin_14
//蜂鸣器PWM时钟TIM以及分频系数
#define TIM_BEEP TIM4
#define TIM_Psc_BEEP 84
//是否为高级定时器
#define TIM_IS_ADVANCED_BEEP 0
//蜂鸣器PWM输出初始化，对应输出PWM的通道修改
#define TIM_OCInit_BEEP		TIM_OC3Init(TIM_BEEP, &TIM_OCInitStructure);TIM_OC3PreloadConfig(TIM_BEEP, TIM_OCPreload_Enable)
//PWM输出比较通道修改
#define BEEP_SOUND  TIM_BEEP->CCR3
//蜂鸣器接口对应时钟
#define BEEP_TUNE   TIM_BEEP->ARR

/**
 *@Function:		Beep_Init()
 *@Description:	蜂鸣器初始化
 *@Param:       形参
 *@Return:	  	返回值
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
	
	#if TIM_IS_ADVANCED_BEEP								//是否为高级定时器，是则需要额外控制PWM输出使能
	TIM_CtrlPWMOutputs(TIM_BEEP,ENABLE);
	#endif	
	
	TIM_Cmd(TIM_BEEP, ENABLE);
}

/**
 *@Function:		Beep_Ctrl(uint16_t tune, uint16_t sound)	
 *@Description:	蜂鸣器控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Beep_Ctrl(uint16_t tune, uint16_t sound)	
{
  BEEP_TUNE = tune;
  BEEP_SOUND = sound;
}
