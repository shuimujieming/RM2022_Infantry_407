#include "nuc.h"
//NUC通信对应串口使能以及GPIO时钟使能
#define RCC_AHBPeriphClockCmd_GPIO_NUC RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE)
#define RCC_APBPeriphClockCmd_USART_NUC RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)
//对应引脚配置
#define GPIO_NUC_1 GPIOA
#define GPIO_PinSource_NUC_1 GPIO_PinSource9
#define GPIO_NUC_2 GPIOB
#define GPIO_PinSource_NUC_2 GPIO_PinSource7
#define GPIO_AF_USART_NUC GPIO_AF_USART1
#define GPIO_Pin_NUC_1 GPIO_Pin_9
#define GPIO_Pin_NUC_2 GPIO_Pin_7
//对应串口以及中断配置
#define USART_NUC USART1
#define USART_NUC_IRQn USART1_IRQn
#define USART_NUC_IRQHandler USART1_IRQHandler
//对应DMA时钟使能
#define RCC_AHBPeriphClockCmd_DMA_NUC RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE)
//DMA接收配置
#define DMA_Channel_NUC_RX DMA_Channel_4
#define DMA_Stream_NUC_RX DMA2_Stream2
//DMA发送配置
#define DMA_Channel_NUC_TX DMA_Channel_4

#define DMA_Stream_NUC_TX DMA2_Stream7
#define DMA_Stream_NUC_TX_IRQn DMA2_Stream7_IRQn
#define DMA_Stream_NUC_TX_IRQHandler DMA2_Stream7_IRQHandler
#define DMA_IT_TCIF_NUC DMA_IT_TCIF7

//NUC通信缓存大小
#define NUC_RX_BUFF_SIZE 7 
#define NUC_TX_BUFF_SIZE 10

//NUC通信数组
uint8_t NUC_rx_buff[2][NUC_RX_BUFF_SIZE];
uint8_t NUC_tx_buff[NUC_TX_BUFF_SIZE];

NUC_Info_TypeDef NUC_Data;
/**
 *@Function:		NUC_USART_Init()
 *@Description:	NUC串口初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void NUC_USART_Init()
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	USART_InitTypeDef  USART_InitStruct;
	DMA_InitTypeDef    DMA_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStruct;
	
	
	RCC_AHBPeriphClockCmd_GPIO_NUC;
	RCC_APBPeriphClockCmd_USART_NUC;
	

	GPIO_PinAFConfig(GPIO_NUC_1, GPIO_PinSource_NUC_1, GPIO_AF_USART_NUC);
	GPIO_PinAFConfig(GPIO_NUC_2, GPIO_PinSource_NUC_2, GPIO_AF_USART_NUC);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_NUC_1;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIO_NUC_1, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_NUC_2;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIO_NUC_2, &GPIO_InitStruct);	
	
	USART_InitStruct.USART_BaudRate            = 115200;
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;
	USART_InitStruct.USART_Parity              = USART_Parity_No;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART_NUC, &USART_InitStruct);
	
	USART_ClearFlag(USART_NUC, USART_FLAG_IDLE);
	USART_ITConfig(USART_NUC, USART_IT_IDLE, ENABLE);	
	
	USART_Cmd(USART_NUC, ENABLE);
}
/**
 *@Function:		NUC_USART_DMA_Init()
 *@Description:	NUC串口DMA初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void NUC_USART_DMA_Init()
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd_DMA_NUC;
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_NUC_RX;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART_NUC->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)NUC_rx_buff[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = NUC_RX_BUFF_SIZE;
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
	DMA_Init(DMA_Stream_NUC_RX, &DMA_InitStructure);
	
	DMA_DoubleBufferModeConfig(DMA_Stream_NUC_RX, (uint32_t)NUC_rx_buff[1], DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA_Stream_NUC_RX, ENABLE);
	
	USART_DMACmd(USART_NUC, USART_DMAReq_Rx, ENABLE);	
	DMA_Cmd(DMA_Stream_NUC_RX, ENABLE);
	

	DMA_InitStructure.DMA_Channel = DMA_Channel_NUC_TX;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART_NUC->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)NUC_tx_buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = NUC_TX_BUFF_SIZE;
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
	DMA_Init(DMA_Stream_NUC_TX, &DMA_InitStructure);
	
	USART_DMACmd(USART_NUC, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(DMA_Stream_NUC_TX,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA_Stream_NUC_TX, ENABLE);

}

/**
 *@Function:		NUC_USART_NVIC_Init()
 *@Description:	NUC串口中断初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void NUC_USART_NVIC_Init()
{
	//串口中断
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART_NUC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//DMA发送中断
	NVIC_InitStructure.NVIC_IRQChannel                   =DMA_Stream_NUC_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                =ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
}
/**
 *@Function:		NUC_Init()
 *@Description:	NUC初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void NUC_Init()
{
	NUC_USART_Init();
	NUC_USART_NVIC_Init();
	NUC_USART_DMA_Init();
}
/**
 *@Function:		DMA_Stream_NUC_TX_IRQHandler(void)
 *@Description:	NUC通信DMA发送中断服务函数
 *@Param:       形参
 *@Return:	  	返回值
 */
void DMA_Stream_NUC_TX_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA_Stream_NUC_TX, DMA_IT_TCIF_NUC) == SET)
	{
		DMA_Cmd(DMA_Stream_NUC_TX, DISABLE);
		DMA_SetCurrDataCounter(DMA_Stream_NUC_TX, NUC_TX_BUFF_SIZE); 
	}
	DMA_ClearITPendingBit(DMA_Stream_NUC_TX, DMA_IT_TCIF_NUC);
}

int Pitch_Angle_NUC = 0;
int Yaw_Angle_NUC = 0;
int Roll_Angle_NUC = 0;

extern float yaw_angle;
//原始Pitch角
extern short Origin_Init_Pitch_Angle;
/**
 *@Function:		NUC_Send_Data()
 *@Description:	NUC数据发送
 *@Param:       形参
 *@Return:	  	返回值
 */
void NUC_Send_Data()
{
	//角度计算
	Pitch_Angle_NUC = (CAN_Gimbal[1].Current_MechAngle - Origin_Init_Pitch_Angle)/8192.0f*36000;
	Yaw_Angle_NUC = (int16_t)((int16_t)(yaw_angle * 100)%36000);
	Roll_Angle_NUC = (int16_t)((int16_t)(imu.rol * 100)%36000);
	
	//最高位代表颜色，0为红色，1为蓝色
	if( game_robot_state.robot_id  <=7)	
	{NUC_tx_buff[8] = NUC_tx_buff[8] & 0x7f;}       //默认己方红色
	else if(game_robot_state.robot_id >= 11)
	{NUC_tx_buff[8] = NUC_tx_buff[8] | 0x80;}       //己方蓝色	
	
	//陀螺模式
	if(DBUS.PC.Keyboard & KEY_F && !DBUS.PC.Keyboard & KEY_CTRL)
	{
		NUC_tx_buff[8] = NUC_tx_buff[8] & 0xf0;
		NUC_tx_buff[8] = NUC_tx_buff[8] | 0x03;
	}
	//打符模式
	if(DBUS.PC.Keyboard & KEY_Z)
	{
		NUC_tx_buff[8] = NUC_tx_buff[8] & 0xf0;
		NUC_tx_buff[8] = NUC_tx_buff[8] | 0x02;
	}
	//普通模式
	if(DBUS.PC.Keyboard & KEY_F && DBUS.PC.Keyboard & KEY_CTRL)
	{
		NUC_tx_buff[8] = NUC_tx_buff[8] & 0xf0;
		NUC_tx_buff[8] = NUC_tx_buff[8] | 0x00;
	}
	
	NUC_tx_buff[0] = 0xff;
	memcpy(&NUC_tx_buff[1],&Pitch_Angle_NUC,2);
	memcpy(&NUC_tx_buff[3],&Yaw_Angle_NUC,2);
	memcpy(&NUC_tx_buff[5],&Roll_Angle_NUC,2);
	NUC_tx_buff[7] = (uint8_t)(real_shoot_data.bullet_speed * 8.0f);
	NUC_tx_buff[8] = (uint8_t)NUC_tx_buff[8];			
	NUC_tx_buff[9] = 0xfe;
	//DMA发送启动
	DMA_Cmd(DMA_Stream_NUC_TX,ENABLE);
}
/**
 *@Function:		NUC_Data_Decode(uint8_t *buff)
 *@Description:	NUC接收数据解析
 *@Param:       形参
 *@Return:	  	返回值
 */
void NUC_Data_Decode(uint8_t *buff)
{
	NUC_Data.header = buff[0];
	NUC_Data.tail = buff[6];
	
	//首尾校验
	if(NUC_Data.header == 0xff && NUC_Data.tail == 0xfe)
	{
		NUC_Data.pit_offset = ((short)(buff[2] << 8 | buff[1]))/1000.0f;
		NUC_Data.yaw_offset = ((short)(buff[4] << 8 | buff[3]))/1000.0f;
		NUC_Data.buff_flag = buff[5];		
	}

}
/**
 *@Function:		USART_NUC_IRQHandler()
 *@Description:	Description：NUC串口中断处理函数
 *@Param:       形参
 *@Return:	  	返回值
 */
void USART_NUC_IRQHandler()
{
			if(USART_GetITStatus(USART_NUC, USART_IT_IDLE) != RESET)	//判断是否为空闲中断
    {
        uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART_NUC);

        if(DMA_GetCurrentMemoryTarget(DMA_Stream_NUC_RX) == DMA_Memory_0)	//获取当前目标内存是否为 DMA_Memory_0
        {
            //重新设置DMA
            DMA_Cmd(DMA_Stream_NUC_RX, DISABLE);
            this_time_rx_len = DMA_GetCurrDataCounter(DMA_Stream_NUC_RX);
            DMA_SetCurrDataCounter(DMA_Stream_NUC_RX, NUC_RX_BUFF_SIZE);
            DMA_Cmd(DMA_Stream_NUC_RX, ENABLE);
            if(this_time_rx_len == NUC_RX_BUFF_SIZE)	//校验接收
            {
                //处理数据
               NUC_Data_Decode(NUC_rx_buff[1]);
            }
        }
        else
        {
            //重新设置DMA
            DMA_Cmd(DMA_Stream_NUC_RX, DISABLE);
            this_time_rx_len = DMA_GetCurrDataCounter(DMA_Stream_NUC_RX);
            DMA_SetCurrDataCounter(DMA_Stream_NUC_RX, NUC_RX_BUFF_SIZE);
            DMA_Cmd(DMA_Stream_NUC_RX, ENABLE);
            if(this_time_rx_len == NUC_RX_BUFF_SIZE)	//校验接收
            {
                //处理数据
               NUC_Data_Decode(NUC_rx_buff[0]);
            }
        }
    }
}