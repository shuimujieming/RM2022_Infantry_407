#include "dr16.h"
//遥控器DR16接收机对应GPIO以及串口配置使能
#define RCC_AHBPeriphClockCmd_GPIO_DR16 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE)
#define RCC_APBPeriphClockCmd_USART_DR16 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE)
#define GPIO_AF_USART_DR16 GPIO_AF_USART3

//对应引脚配置
#define GPIO_DR16 GPIOC
#define GPIO_PinSource_DR16 GPIO_PinSource11
#define GPIO_Pin_DR16 GPIO_Pin_11
//串口号修改以及对应中断向量和服务函数名
#define USART_DR16 USART3
#define USART_NUC_IRQn USART3_IRQn
#define USART_DR16_IRQHandler USART3_IRQHandler
//对应串口接收的DMA使能以及配置
#define RCC_AHBPeriphClockCmd_DMA_DR16 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE)
#define DMA_Stream_DR16 DMA1_Stream1
#define DMA_Channel_DR16 DMA_Channel_4

//一帧遥控器数据的长度18字节
#define DBUS_RX_BUFF_SIZE 18
//遥控器数据接收缓冲数组，双缓冲
static uint8_t DBUS_rx_buff[2][DBUS_RX_BUFF_SIZE];
//接收机数据结构体
DBUS_DecodingData_TypeDef DBUS;

//DR16信号量
int DR16_Signal = 0;

/**
 *@Function:		DR16_USART_Init()
 *@Description:	DR16串口初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void DR16_USART_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHBPeriphClockCmd_GPIO_DR16;
	RCC_APBPeriphClockCmd_USART_DR16;

	GPIO_PinAFConfig(GPIO_DR16,GPIO_PinSource_DR16,GPIO_AF_USART_DR16); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DR16; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIO_DR16,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = 100000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_Init(USART_DR16, &USART_InitStructure);

	USART_ClearFlag(USART_DR16, USART_FLAG_IDLE);
	USART_ITConfig(USART_DR16, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART_DR16, ENABLE);
}
/**
 *@Function:		DR16_USART_DMA_Init()
 *@Description:	DR16串口DMA初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void DR16_USART_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd_DMA_DR16;

	DMA_DeInit(DMA_Stream_DR16);

	DMA_InitStructure.DMA_Channel = DMA_Channel_DR16;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART_DR16->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DBUS_rx_buff[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = DBUS_RX_BUFF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_Stream_DR16, &DMA_InitStructure);
	
	DMA_DoubleBufferModeConfig(DMA_Stream_DR16, (uint32_t)DBUS_rx_buff[1], DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA_Stream_DR16, ENABLE);
	
	DMA_Cmd(DMA_Stream_DR16, ENABLE);
	
	USART_DMACmd(USART_DR16, USART_DMAReq_Rx, ENABLE);
}
/**
 *@Function:		DR16_USART_NVIC_Init()
 *@Description:	DR16串口中断优先级初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void DR16_USART_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART_NUC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/**
 *@Function:		DR16_Init()
 *@Description:	DR16初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void DR16_Init()
{
	DR16_USART_Init();
	DR16_USART_NVIC_Init();
	DR16_USART_DMA_Init();
}

/**
 *@Function:		DR16_Data_Decode(volatile const uint8_t *dbus_rx_buff)
 *@Description:	DR16数据解析
 *@Param:       形参
 *@Return:	  	返回值
 */
void DR16_Data_Decode(volatile const uint8_t *dbus_rx_buff)
{
	//遥控器信号量填充
	DR16_Signal = 100;
	
	DBUS.RC.ch0          = ((( dbus_rx_buff[0] | (dbus_rx_buff [1]<<8) ) & 0x07FF)-1024);                     
	DBUS.RC.ch1          = ((( dbus_rx_buff[1]>>3 | dbus_rx_buff[2]<<5 ) & 0x07FF)-1024);                      
	DBUS.RC.ch2          = ((( dbus_rx_buff[2]>>6 | dbus_rx_buff[3]<<2 | dbus_rx_buff[4]<<10 ) & 0x07FF)-1024); 
	DBUS.RC.ch3          = (( dbus_rx_buff[4]>>1 | dbus_rx_buff[5]<<7 ) & 0x07FF)-1024;                        
	DBUS.RC.ch4          = ((( dbus_rx_buff[16] | (dbus_rx_buff [17]<<8) ) & 0x07FF)-1024);                         

	DBUS.RC.Switch_Left  = (( dbus_rx_buff[5]>>4 ) & 0x00C )>>2;                                             
	DBUS.RC.Switch_Right = ( dbus_rx_buff[5]>>4 ) & 0x003;                                                   
	
	DBUS.PC.X    				 = dbus_rx_buff[6] | (dbus_rx_buff[7] << 8);                                            
	DBUS.PC.Y            = dbus_rx_buff[8] | (dbus_rx_buff[9] << 8); 
	DBUS.PC.Z            = dbus_rx_buff[10] | (dbus_rx_buff[11] << 8);                                            

	DBUS.PC.Press_Left   = dbus_rx_buff[12];                                                                   
	DBUS.PC.Press_Right  = dbus_rx_buff[13];                                                                   
	DBUS.PC.Keyboard     = dbus_rx_buff[14] | dbus_rx_buff[15] << 8;                                           
	
}
/**
 *@Function:		USART_DR16_IRQHandler(void)
 *@Description:	DR16串口中断服务函数
 *@Param:       形参
 *@Return:	  	返回值
 */
void USART_DR16_IRQHandler(void)
{
		if(USART_GetITStatus(USART_DR16, USART_IT_IDLE) != RESET)	//判断是否为空闲中断
    {
        uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART_DR16);

        if(DMA_GetCurrentMemoryTarget(DMA_Stream_DR16) == DMA_Memory_0)	//获取当前目标内存是否为 DMA_Memory_0
        {
            //重新设置DMA
            DMA_Cmd(DMA_Stream_DR16, DISABLE);
            this_time_rx_len = DMA_GetCurrDataCounter(DMA_Stream_DR16);
            DMA_SetCurrDataCounter(DMA_Stream_DR16, DBUS_RX_BUFF_SIZE);
            DMA_Cmd(DMA_Stream_DR16, ENABLE);
            if(this_time_rx_len == DBUS_RX_BUFF_SIZE)	//校验接收成功18个字节长度
            {
                //处理遥控器数据
                DR16_Data_Decode(DBUS_rx_buff[1]);
            }
        }
        else
        {
            //重新设置DMA
            DMA_Cmd(DMA_Stream_DR16, DISABLE);
            this_time_rx_len = DMA_GetCurrDataCounter(DMA_Stream_DR16);
            DMA_SetCurrDataCounter(DMA_Stream_DR16, DBUS_RX_BUFF_SIZE);
            DMA_Cmd(DMA_Stream_DR16, ENABLE);
            if(this_time_rx_len == DBUS_RX_BUFF_SIZE)	//校验接收成功18个字节长度
            {
                //处理遥控器数据
                DR16_Data_Decode(DBUS_rx_buff[0]);
            }
        }
    }
}

