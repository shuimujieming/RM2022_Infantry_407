#include "bmi088.h"

/**
 *@Function:		BMI088_SPI_Init()
 *@Description:	BMI088硬件SPI初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void BMI088_SPI_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	//PA7(MOSI)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//PB3(CLK) PB4(MISO)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//PA4(CS1_ACCEL_Pin)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	
	//PB0(CS1_GYRO_Pin)
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	
	//引脚复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//全双工模式
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;//主机模式
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;//波特率预分频值为128
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;//空闲状态为低电平;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;//第一个跳变沿
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//数据从MSB开始
	SPI_InitStructure.SPI_CRCPolynomial=10;
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);//使能SPI1
}

