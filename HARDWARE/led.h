#ifndef __LED_H
#define __LED_H
#include "main.h"


//LED�˿ڶ���
#define LED_RED_ON PHout(12)=1	//���
#define LED_RED_OFF PHout(12)=0	//���


#define LED_GREEN_ON PHout(11)=1	//�̵�	 
#define LED_GREEN_OFF PHout(11)=0	//�̵�	 

#define LED_BLUE_ON PHout(10)=1	//����	 
#define LED_BLUE_OFF PHout(10)=0	//����	

#define LED_RED_TOGGLE 		GPIO_ToggleBits(GPIOH,GPIO_Pin_12)	//���
#define LED_GREEN_TOGGLE 	GPIO_ToggleBits(GPIOH,GPIO_Pin_11)	//�̵�	 
#define LED_BLUE_TOGGLE 	GPIO_ToggleBits(GPIOH,GPIO_Pin_10)	//����	 

void LED_Init();//��ʼ��LED��			    
void LED_Run(void);

#endif
