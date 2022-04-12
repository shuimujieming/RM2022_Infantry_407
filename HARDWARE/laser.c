#include "laser.h"

//激光对应的GPIO时钟使能
#define RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_LASER, ENABLE) 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
//对应引脚配置
#define GPIO_Pin_LASER GPIO_Pin_8
#define GPIO_LASER GPIOC

/**
 *@Function:		Laser_Init(void)
 *@Description:	激光初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void Laser_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_LASER, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LASER;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIO_LASER, &GPIO_InitStructure); 
	
	GPIO_SetBits(GPIO_LASER,GPIO_Pin_LASER);
	
}

/**
 *@Function:		Limit_Init()
 *@Description:	限位初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void Limit_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
		
}
