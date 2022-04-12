#include "can.h"

/*********************************************
Copyright(C) 水木皆Ming
All rights reserved
File name:	can.c
Author:		shuimujieming
Version:		1.0
Description:	CAN初始化
Others:		
Log:	
*********************************************/
//CAN1对应引脚以及时钟使能
#define RCC_AHBPeriphClockCmd_GPIO_CAN1 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
#define GPIO_CAN1 GPIOD
#define GPIO_PinSource_CAN1_1 GPIO_PinSource0
#define GPIO_PinSource_CAN1_2 GPIO_PinSource1
#define GPIO_Pin_CAN1_1 GPIO_Pin_0
#define GPIO_Pin_CAN1_2 GPIO_Pin_1
//CAN2对应引脚以及时钟使能
#define RCC_AHBPeriphClockCmd_GPIO_CAN2 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
#define GPIO_CAN2 GPIOB
#define GPIO_PinSource_CAN2_1 GPIO_PinSource5
#define GPIO_PinSource_CAN2_2 GPIO_PinSource6
#define GPIO_Pin_CAN2_1 GPIO_Pin_5
#define GPIO_Pin_CAN2_2 GPIO_Pin_6
/*
*@Description：CAN1初始化
*@param 1：	  参数1
*@param 2：	  参数2
*@return:：	  返回值
*/
/**
 *@Function:		CAN1_Init(void)
 *@Description:	CAN1初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void CAN1_Init(void)
{
	GPIO_InitTypeDef      GPIO_InitStruct;
	NVIC_InitTypeDef      NVIC_InitStruct;
	CAN_InitTypeDef       CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	RCC_AHBPeriphClockCmd_GPIO_CAN1;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	GPIO_PinAFConfig(GPIO_CAN1, GPIO_PinSource_CAN1_1, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIO_CAN1, GPIO_PinSource_CAN1_2, GPIO_AF_CAN1);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_CAN1_1 | GPIO_Pin_CAN1_2;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIO_CAN1,&GPIO_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel                    = CAN1_RX0_IRQn;            
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE ;
	NVIC_Init(&NVIC_InitStruct);
	
  NVIC_InitStruct.NVIC_IRQChannel                    = CAN1_TX_IRQn;            
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE ;
	NVIC_Init(&NVIC_InitStruct);
  
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStruct);
  
	//波特率为42/(1+9+4)/3
	CAN_InitStruct.CAN_Prescaler = 3;                                               //CAN BaudRate 42/(1+9+4)/3=1Mbps  //分频系数(Fdiv)为3+1
	CAN_InitStruct.CAN_Mode      = CAN_Mode_Normal;                                 //模式设置 
	CAN_InitStruct.CAN_SJW       = CAN_SJW_1tq;                                     //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStruct.CAN_BS1       = CAN_BS1_9tq;                                     //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStruct.CAN_BS2       = CAN_BS2_4tq;                                     //Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStruct.CAN_TTCM      = DISABLE;                                         //非时间触发通信模式
	CAN_InitStruct.CAN_ABOM      = DISABLE;                                         //软件自动离线管理	  
	CAN_InitStruct.CAN_AWUM      = DISABLE;                                         //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStruct.CAN_NART      = DISABLE;	                                        //禁止报文自动传送
	CAN_InitStruct.CAN_RFLM      = DISABLE;                                         //报文不锁定,新的覆盖旧的
	CAN_InitStruct.CAN_TXFP      = ENABLE;	                                        //优先级由报文标识符决定
	CAN_Init(CAN1,&CAN_InitStruct);
	
	//CAN过滤器设置
	CAN_FilterInitStruct.CAN_FilterIdHigh         = 0x0000;                         //32位ID
	CAN_FilterInitStruct.CAN_FilterIdLow          = 0x0000;                         //32位ID
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh     = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow      = 0x0000;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;               //过滤器0关联到FIFO0;
	CAN_FilterInitStruct.CAN_FilterNumber         = 0;                              //过滤器0,范围0---13
	CAN_FilterInitStruct.CAN_FilterMode           = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale          = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterActivation     = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                                          //FIFO0消息挂号中断允许，打开接收中断
	CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);                                           //打开发送中断

}
/**
 *@Function:		CAN2_Init(void)
 *@Description:	CAN2初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void CAN2_Init(void)
{
	GPIO_InitTypeDef      GPIO_InitStruct;
	NVIC_InitTypeDef      NVIC_InitStruct;
	CAN_InitTypeDef       CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	RCC_AHBPeriphClockCmd_GPIO_CAN2;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); //先CAN1 后CAN2 时钟挂载要求
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	
	GPIO_PinAFConfig(GPIO_CAN2, GPIO_PinSource_CAN2_1, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIO_CAN2, GPIO_PinSource_CAN2_2, GPIO_AF_CAN2);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_CAN2_1 | GPIO_Pin_CAN2_2;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIO_CAN2,&GPIO_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel                    = CAN2_RX0_IRQn;    
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE ;
	NVIC_Init(&NVIC_InitStruct);
	
  NVIC_InitStruct.NVIC_IRQChannel                    = CAN2_TX_IRQn;     
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE ;
	NVIC_Init(&NVIC_InitStruct);
  
  CAN_DeInit(CAN2);
  CAN_StructInit(&CAN_InitStruct);
    
	CAN_InitStruct.CAN_Prescaler = 3;                                               //CAN BaudRate 42/(1+9+4)/3=1Mbps  //分频系数(Fdiv)为3+1
	CAN_InitStruct.CAN_Mode      = CAN_Mode_Normal;                                 //模式设置 
	CAN_InitStruct.CAN_SJW       = CAN_SJW_1tq;                                     //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStruct.CAN_BS1       = CAN_BS1_9tq;                                     //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStruct.CAN_BS2       = CAN_BS2_4tq;                                     //Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStruct.CAN_TTCM      = DISABLE;                                         //非时间触发通信模式
	CAN_InitStruct.CAN_ABOM      = DISABLE;                                         //软件自动离线管理	  
	CAN_InitStruct.CAN_AWUM      = DISABLE;                                         //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStruct.CAN_NART      = DISABLE;	                                        //禁止报文自动传送
	CAN_InitStruct.CAN_RFLM      = DISABLE;                                         //报文不锁定,新的覆盖旧的
	CAN_InitStruct.CAN_TXFP      = ENABLE;	                                        //优先级由报文标识符决定
	CAN_Init(CAN2,&CAN_InitStruct);
	
	CAN_FilterInitStruct.CAN_FilterIdHigh         = 0x0000;                         //32位ID
	CAN_FilterInitStruct.CAN_FilterIdLow          = 0x0000;                         //32位ID
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh     = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow      = 0x0000;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;               //过滤器0关联到FIFO0;
	CAN_FilterInitStruct.CAN_FilterNumber         = 14;                              //过滤器0,范围0---13
	CAN_FilterInitStruct.CAN_FilterMode           = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale          = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterActivation     = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);                                          //FIFO0消息挂号中断允许，打开接收中断
	CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);          
}

/**
 *@Function:		CAN12_Init(void)
 *@Description:	CAN1和CAN2初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void CAN12_Init(void)
{
	CAN1_Init();
	CAN2_Init();
}
