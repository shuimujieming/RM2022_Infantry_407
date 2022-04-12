#include "referee.h"

//����ϵͳ����ʱ���Լ�GPIOʱ��ʹ��
#define RCC_AHBPeriphClockCmd_GPIO_Referee RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE)
#define RCC_APBPeriphClockCmd_USART_Referee RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE)
//����ϵͳ��������
#define GPIO_Referee GPIOG
#define GPIO_PinSource_Referee_1 GPIO_PinSource9
#define GPIO_PinSource_Referee_2 GPIO_PinSource14
#define GPIO_AF_USART_Referee GPIO_AF_USART6
//����ϵͳ������������
#define GPIO_Pin_Referee_1 GPIO_Pin_9
#define GPIO_Pin_Referee_2 GPIO_Pin_14
//����ϵͳ�����ж�����
#define USART_Referee USART6
#define USART_Referee_IRQn USART6_IRQn
#define USART_Referee_IRQHandler USART6_IRQHandler
//����ϵͳDMAʹ���Լ�����
#define RCC_AHBPeriphClockCmd_DMA_Referee RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE)
#define DMA_Channel_Referee_RX DMA_Channel_5
#define DMA_Stream_Referee_RX DMA2_Stream1

#define DMA_Channel_Referee_TX DMA_Channel_5
#define DMA_Stream_Referee_TX DMA2_Stream6
#define DMA_Stream_Referee_TX_IRQn DMA2_Stream6_IRQn
#define DMA_Stream_Referee_TX_IRQHandler DMA2_Stream6_IRQHandler
#define DMA_IT_TCIF_Referee DMA_IT_TCIF6


//����3�������ݻ���
uint8_t Usart_Referee_Rx_Buf[2][1024];
uint8_t Usart_Referee_Tx_Buf[MAX_SIZE];


/**
 *@Function:		Referee_USART_Init()
 *@Description:	����ϵͳ���ڳ�ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Referee_USART_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHBPeriphClockCmd_GPIO_Referee; 
	RCC_APBPeriphClockCmd_USART_Referee;

	GPIO_PinAFConfig(GPIO_Referee,GPIO_PinSource_Referee_1,GPIO_AF_USART_Referee); 
	GPIO_PinAFConfig(GPIO_Referee,GPIO_PinSource_Referee_2,GPIO_AF_USART_Referee); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Referee_1|GPIO_Pin_Referee_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_Referee,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;//
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_Referee, &USART_InitStructure);

	USART_ClearFlag(USART_Referee, USART_FLAG_IDLE);
	USART_ITConfig(USART_Referee, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART_Referee, ENABLE);
}
/**
 *@Function:		Referee_USART_DMA_Init()
 *@Description:	����ϵͳ����DMA��ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Referee_USART_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd_DMA_Referee;
	

	DMA_InitStructure.DMA_Channel = DMA_Channel_Referee_RX;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART_Referee->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Usart_Referee_Rx_Buf[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1024;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_Stream_Referee_RX, &DMA_InitStructure);
	
	DMA_DoubleBufferModeConfig(DMA_Stream_Referee_RX, (uint32_t)Usart_Referee_Rx_Buf[1], DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA_Stream_Referee_RX, ENABLE);
	USART_DMACmd(USART_Referee, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA_Stream_Referee_RX,ENABLE);


	DMA_InitStructure.DMA_Channel = DMA_Channel_Referee_TX;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART_Referee->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Usart_Referee_Tx_Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = MAX_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_Stream_Referee_TX, &DMA_InitStructure);
	
	USART_DMACmd(USART_Referee, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(DMA_Stream_Referee_TX,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA_Stream_Referee_TX,ENABLE);	
}
/**
 *@Function:		Referee_USART_NVIC_Init()
 *@Description:	����ϵͳ�����жϳ�ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Referee_USART_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART_Referee_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//DMA�����ж�
	NVIC_InitStructure.NVIC_IRQChannel                   =DMA_Stream_Referee_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                =ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
}
/**
 *@Function:		Referee_Init()
 *@Description:	����ϵͳ��ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Referee_Init()
{
	Referee_USART_Init();
	Referee_USART_NVIC_Init();
	Referee_USART_DMA_Init();
}

/**
 *@Function:		USART_Referee_IRQHandler(void)
 *@Description:	����ϵͳ�����жϷ�����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void USART_Referee_IRQHandler(void)
{	
		if(USART_GetITStatus(USART_Referee, USART_IT_IDLE) != RESET)	//�ж��Ƿ�Ϊ�����ж�
    {
			u8 selectedMemory = 0;	//��ǰѡ����ڴ�����	
			u16 index=0;	//��ǰ�������
			u16 data_length;	//֡���ݳ���
			
      (void)USART_Referee->SR;
			(void)USART_Referee->DR;
			
			DMA_Cmd(DMA_Stream_Referee_RX,DISABLE);
			
			if(DMA_GetCurrentMemoryTarget(DMA_Stream_Referee_RX) == DMA_Memory_0)	//��ȡ��ǰĿ���ڴ��Ƿ�Ϊ DMA_Memory_0
			{
				selectedMemory = 0;
				//����ϵͳ��������ѡ����
				while(Usart_Referee_Rx_Buf[selectedMemory][index]==0XA5) //֡ͷSOFУ��
				{
						if(Verify_CRC8_Check_Sum(&Usart_Referee_Rx_Buf[selectedMemory][index],5) == 1)
						{
							data_length = Usart_Referee_Rx_Buf[selectedMemory][index+2]<<8 | Usart_Referee_Rx_Buf[selectedMemory][index+1] + 9;
							if(Verify_CRC16_Check_Sum(&Usart_Referee_Rx_Buf[selectedMemory][index],data_length))	//CRC16У�飨CRC8������У�飩
							{
								//����ϵͳ���ݽ���
								RefereeInfo_Decode(&Usart_Referee_Rx_Buf[selectedMemory][index]);	
							}			
						}
						index+=data_length;
				}
				
				//�������
				memset(Usart_Referee_Rx_Buf[selectedMemory],0,1024);
				
				
				//�������DMA������
				DMA_SetCurrDataCounter(DMA_Stream_Referee_RX,1024);
				
				DMA_Stream_Referee_RX->CR |= (uint32_t)(DMA_SxCR_CT);
								
				DMA_Cmd(DMA_Stream_Referee_RX,ENABLE);

			}
			else
			{
				selectedMemory = 1;
				
											//����ϵͳ��������ѡ����
				while(Usart_Referee_Rx_Buf[selectedMemory][index]==0XA5) //֡ͷSOFУ��
				{
						if(Verify_CRC8_Check_Sum(&Usart_Referee_Rx_Buf[selectedMemory][index],5) == 1)
						{
							data_length = Usart_Referee_Rx_Buf[selectedMemory][index+2]<<8 | Usart_Referee_Rx_Buf[selectedMemory][index+1] + 9;
							if(Verify_CRC16_Check_Sum(&Usart_Referee_Rx_Buf[selectedMemory][index],data_length))	//CRC16У�飨CRC8������У�飩
							{
								//����ϵͳ���ݽ���
								RefereeInfo_Decode(&Usart_Referee_Rx_Buf[selectedMemory][index]);	
							}			
						}
						index+=data_length;
				}
				
				//�������
				memset(Usart_Referee_Rx_Buf[selectedMemory],0,1024);
				
				//�������DMA������
				DMA_SetCurrDataCounter(DMA_Stream_Referee_RX,1024);
				
				DMA_Stream_Referee_RX->CR &= ~(uint32_t)(DMA_SxCR_CT);  
				
				DMA_Cmd(DMA_Stream_Referee_RX,ENABLE);

			}
			
    }
}

/**
  * @brief     pack data to bottom device
  * @param[in] sof��framehearder
  * @param[in] cmd_id:  command id of data
  * @param[in] *p_data: pointer to the data to be sent
  * @param[in] len:     the data length
  */
void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
	static uint8_t seq = 0;
	unsigned char i=i;
	uint8_t tx_buff[MAX_SIZE];
	
  uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //����֡����	

	memset(tx_buff,0,frame_length);  //�洢���ݵ���������
	
	/*****֡ͷ���*****/
	tx_buff[0] = sof;//����֡��ʼ�ֽ�
	memcpy(&tx_buff[1],(uint8_t*)&len, sizeof(len));//����֡��data�ĳ���
	tx_buff[3] = seq;//�����
	Append_CRC8_Check_Sum(tx_buff,frameheader_len);  //֡ͷУ��CRC8

	/*****��������*****/
	memcpy(&tx_buff[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
	
	/*****���ݴ��*****/
	memcpy(&tx_buff[frameheader_len+cmd_len], p_data, len);
	Append_CRC16_Check_Sum(tx_buff,frame_length);  //һ֡����У��CRC16

	if (seq == 0xff) seq=0;
  else seq++;

	
	/*****�����ϴ�*****/
	USART_ClearFlag(USART_Referee,USART_FLAG_TC);
	for(i=0;i<frame_length;i++)
	{
	  USART_SendData(USART_Referee,tx_buff[i]);
	  while (USART_GetFlagStatus(USART_Referee,USART_FLAG_TC) == RESET); //�ȴ�֮ǰ���ַ��������
	}
	
//	memcpy(Usart_Referee_Tx_Buf,tx_buff,MAX_SIZE);
//	DMA_Cmd(DMA_Stream_Referee_TX, ENABLE);
//	while(DMA_GetFlagStatus(DMA_Stream_Referee_TX,DMA_FLAG_TCIF6)==RESET);
//	DMA_ClearFlag(DMA_Stream_Referee_TX,DMA_FLAG_TCIF6);
//	DMA_SetCurrDataCounter(DMA_Stream_Referee_TX, MAX_SIZE);	
}
/**
 *@Function:		DMA_Stream_Referee_TX_IRQHandler(void)
 *@Description:	����ϵͳͨ��DMA�����жϷ�����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void DMA_Stream_Referee_TX_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA_Stream_Referee_TX, DMA_IT_TCIF_Referee) == SET)
	{
		DMA_Cmd(DMA_Stream_Referee_TX, DISABLE);
		DMA_SetCurrDataCounter(DMA_Stream_Referee_TX, MAX_SIZE);
	}
	DMA_ClearITPendingBit(DMA_Stream_Referee_TX, DMA_IT_TCIF_Referee);
}