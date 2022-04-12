#include "laser.h"

//�����Ӧ��GPIOʱ��ʹ��
#define RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_LASER, ENABLE) 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
//��Ӧ��������
#define GPIO_Pin_LASER GPIO_Pin_8
#define GPIO_LASER GPIOC

/**
 *@Function:		Laser_Init(void)
 *@Description:	�����ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Laser_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_LASER, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LASER;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIO_LASER, &GPIO_InitStructure); 
	
	GPIO_SetBits(GPIO_LASER,GPIO_Pin_LASER);
	
}

/**
 *@Function:		Limit_Init()
 *@Description:	��λ��ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Limit_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
		
}
