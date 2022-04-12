#include "led.h" 


/**
 *@Function:		LED_Init()
 *@Description:	LED初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void LED_Init()
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//使能GPIOF时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOH, &GPIO_InitStructure);//LED_G 
	
	
	//灭灯
	GPIO_ResetBits(GPIOH,GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);
}

/**
 *@Function:		LED_Run(void)
 *@Description:	LED运行
 *@Param:       形参
 *@Return:	  	返回值
 */
void LED_Run(void)
{
	GPIO_ToggleBits(GPIOH,GPIO_Pin_11);
}






