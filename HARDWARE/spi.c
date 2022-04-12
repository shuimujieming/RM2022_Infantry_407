#include "spi.h"

void SPI_IMU_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5,ENABLE);
	
	//PF6(NSS)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	//PF7(SPI5_SCK) PF8(SPI5_MISO) PF9(SPI5_MOSI)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	//引脚复用
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_SPI5);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5,ENABLE);//复位SPI5
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5,DISABLE);//停止复位SPI5
	
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//全双工模式
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;//主机模式
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;//波特率预分频值为128
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;//空闲状态为低电平;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;//第一个跳变沿
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//数据从MSB开始
	SPI_InitStructure.SPI_CRCPolynomial=10;
	SPI_Init(SPI5,&SPI_InitStructure);
	
	SPI_Cmd(SPI5,ENABLE);//使能SPI5
}

/*
*@title：IMU读写一个字节
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
u8 SPI_ReadWriteByte(u8 TxData)
{
	while(SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_TXE)==RESET){}
	SPI_I2S_SendData(SPI5,TxData);
	while(SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_RXNE)==RESET){}
	return SPI_I2S_ReceiveData(SPI5);
}