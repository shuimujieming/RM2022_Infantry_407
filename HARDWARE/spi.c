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
	
	//���Ÿ���
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_SPI5);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5,ENABLE);//��λSPI5
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI5,DISABLE);//ֹͣ��λSPI5
	
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//ȫ˫��ģʽ
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;//����ģʽ
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;//������Ԥ��ƵֵΪ128
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;//����״̬Ϊ�͵�ƽ;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;//��һ��������
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//���ݴ�MSB��ʼ
	SPI_InitStructure.SPI_CRCPolynomial=10;
	SPI_Init(SPI5,&SPI_InitStructure);
	
	SPI_Cmd(SPI5,ENABLE);//ʹ��SPI5
}

/*
*@title��IMU��дһ���ֽ�
*@description��
*@param 1��	
*@param 2��	
*@return:��	
*/
u8 SPI_ReadWriteByte(u8 TxData)
{
	while(SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_TXE)==RESET){}
	SPI_I2S_SendData(SPI5,TxData);
	while(SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_RXNE)==RESET){}
	return SPI_I2S_ReceiveData(SPI5);
}