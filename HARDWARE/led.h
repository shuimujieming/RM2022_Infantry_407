#ifndef __LED_H
#define __LED_H
#include "main.h"


//LED¶Ë¿Ú¶¨Òå
#define LED_RED_ON PHout(12)=1	//ºìµÆ
#define LED_RED_OFF PHout(12)=0	//ºìµÆ


#define LED_GREEN_ON PHout(11)=1	//ÂÌµÆ	 
#define LED_GREEN_OFF PHout(11)=0	//ÂÌµÆ	 

#define LED_BLUE_ON PHout(10)=1	//À¶µÆ	 
#define LED_BLUE_OFF PHout(10)=0	//À¶µÆ	

#define LED_RED_TOGGLE 		GPIO_ToggleBits(GPIOH,GPIO_Pin_12)	//ºìµÆ
#define LED_GREEN_TOGGLE 	GPIO_ToggleBits(GPIOH,GPIO_Pin_11)	//ÂÌµÆ	 
#define LED_BLUE_TOGGLE 	GPIO_ToggleBits(GPIOH,GPIO_Pin_10)	//À¶µÆ	 

void LED_Init();//³õÊ¼»¯LEDµÆ			    
void LED_Run(void);

#endif
