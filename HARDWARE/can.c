#include "can.h"

/*********************************************
Copyright(C) ˮľ��Ming
All rights reserved
File name:	can.c
Author:		shuimujieming
Version:		1.0
Description:	CAN��ʼ��
Others:		
Log:	
*********************************************/
//CAN1��Ӧ�����Լ�ʱ��ʹ��
#define RCC_AHBPeriphClockCmd_GPIO_CAN1 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
#define GPIO_CAN1 GPIOD
#define GPIO_PinSource_CAN1_1 GPIO_PinSource0
#define GPIO_PinSource_CAN1_2 GPIO_PinSource1
#define GPIO_Pin_CAN1_1 GPIO_Pin_0
#define GPIO_Pin_CAN1_2 GPIO_Pin_1
//CAN2��Ӧ�����Լ�ʱ��ʹ��
#define RCC_AHBPeriphClockCmd_GPIO_CAN2 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
#define GPIO_CAN2 GPIOB
#define GPIO_PinSource_CAN2_1 GPIO_PinSource5
#define GPIO_PinSource_CAN2_2 GPIO_PinSource6
#define GPIO_Pin_CAN2_1 GPIO_Pin_5
#define GPIO_Pin_CAN2_2 GPIO_Pin_6
/*
*@Description��CAN1��ʼ��
*@param 1��	  ����1
*@param 2��	  ����2
*@return:��	  ����ֵ
*/
/**
 *@Function:		CAN1_Init(void)
 *@Description:	CAN1��ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
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
  
	//������Ϊ42/(1+9+4)/3
	CAN_InitStruct.CAN_Prescaler = 3;                                               //CAN BaudRate 42/(1+9+4)/3=1Mbps  //��Ƶϵ��(Fdiv)Ϊ3+1
	CAN_InitStruct.CAN_Mode      = CAN_Mode_Normal;                                 //ģʽ���� 
	CAN_InitStruct.CAN_SJW       = CAN_SJW_1tq;                                     //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStruct.CAN_BS1       = CAN_BS1_9tq;                                     //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStruct.CAN_BS2       = CAN_BS2_4tq;                                     //Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStruct.CAN_TTCM      = DISABLE;                                         //��ʱ�䴥��ͨ��ģʽ
	CAN_InitStruct.CAN_ABOM      = DISABLE;                                         //����Զ����߹���	  
	CAN_InitStruct.CAN_AWUM      = DISABLE;                                         //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStruct.CAN_NART      = DISABLE;	                                        //��ֹ�����Զ�����
	CAN_InitStruct.CAN_RFLM      = DISABLE;                                         //���Ĳ�����,�µĸ��Ǿɵ�
	CAN_InitStruct.CAN_TXFP      = ENABLE;	                                        //���ȼ��ɱ��ı�ʶ������
	CAN_Init(CAN1,&CAN_InitStruct);
	
	//CAN����������
	CAN_FilterInitStruct.CAN_FilterIdHigh         = 0x0000;                         //32λID
	CAN_FilterInitStruct.CAN_FilterIdLow          = 0x0000;                         //32λID
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh     = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow      = 0x0000;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;               //������0������FIFO0;
	CAN_FilterInitStruct.CAN_FilterNumber         = 0;                              //������0,��Χ0---13
	CAN_FilterInitStruct.CAN_FilterMode           = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale          = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterActivation     = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                                          //FIFO0��Ϣ�Һ��ж������򿪽����ж�
	CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);                                           //�򿪷����ж�

}
/**
 *@Function:		CAN2_Init(void)
 *@Description:	CAN2��ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN2_Init(void)
{
	GPIO_InitTypeDef      GPIO_InitStruct;
	NVIC_InitTypeDef      NVIC_InitStruct;
	CAN_InitTypeDef       CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	RCC_AHBPeriphClockCmd_GPIO_CAN2;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); //��CAN1 ��CAN2 ʱ�ӹ���Ҫ��
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
    
	CAN_InitStruct.CAN_Prescaler = 3;                                               //CAN BaudRate 42/(1+9+4)/3=1Mbps  //��Ƶϵ��(Fdiv)Ϊ3+1
	CAN_InitStruct.CAN_Mode      = CAN_Mode_Normal;                                 //ģʽ���� 
	CAN_InitStruct.CAN_SJW       = CAN_SJW_1tq;                                     //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStruct.CAN_BS1       = CAN_BS1_9tq;                                     //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStruct.CAN_BS2       = CAN_BS2_4tq;                                     //Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStruct.CAN_TTCM      = DISABLE;                                         //��ʱ�䴥��ͨ��ģʽ
	CAN_InitStruct.CAN_ABOM      = DISABLE;                                         //����Զ����߹���	  
	CAN_InitStruct.CAN_AWUM      = DISABLE;                                         //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStruct.CAN_NART      = DISABLE;	                                        //��ֹ�����Զ�����
	CAN_InitStruct.CAN_RFLM      = DISABLE;                                         //���Ĳ�����,�µĸ��Ǿɵ�
	CAN_InitStruct.CAN_TXFP      = ENABLE;	                                        //���ȼ��ɱ��ı�ʶ������
	CAN_Init(CAN2,&CAN_InitStruct);
	
	CAN_FilterInitStruct.CAN_FilterIdHigh         = 0x0000;                         //32λID
	CAN_FilterInitStruct.CAN_FilterIdLow          = 0x0000;                         //32λID
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh     = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow      = 0x0000;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;               //������0������FIFO0;
	CAN_FilterInitStruct.CAN_FilterNumber         = 14;                              //������0,��Χ0---13
	CAN_FilterInitStruct.CAN_FilterMode           = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale          = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterActivation     = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);
	
  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);                                          //FIFO0��Ϣ�Һ��ж������򿪽����ж�
	CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);          
}

/**
 *@Function:		CAN12_Init(void)
 *@Description:	CAN1��CAN2��ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN12_Init(void)
{
	CAN1_Init();
	CAN2_Init();
}
