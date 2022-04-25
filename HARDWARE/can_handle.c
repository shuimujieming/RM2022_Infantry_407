#include "can_handle.h"

/*********************************************
Copyright(C) ˮľ��Ming
All rights reserved
File name:	can_handle.h
Author:		shuimujieming
Version:		1.0
Description:	CAN���ݴ�����շ���
Others:		
Log:	
*********************************************/

//�����CAN���ݽṹ���������

//CAN1
//����
CAN_Data_TypeDef CAN_Chassis[4]; //0x201 - 0x204

//CAN2
//��
CAN_Data_TypeDef CAN_Rudder[4]; //0x205-0x208

//��̨
CAN_Data_TypeDef CAN_Gimbal[2];	 //Yaw-0x205 Pitch-0x209
//����
CAN_Data_TypeDef CAN_Shoot[2];	 //Left_Fric-0x203 Right_Fric-0x204
CAN_Data_TypeDef CAN_Trigger;    //Trigger-0x202 

//��������
SuperCap_TypeDef SuperCap_Info;  //SuperCap-0x210

int CAN1_Signal = 0;																		//CAN1�ź�����־
int CAN2_Signal = 0;																		//CAN2�ź�����־
int SuperCap_Signal = 0;																//���������ź�����־

unsigned char CAN1_Tx_Message_Flag = 0;									//CAN1������Ϣ��־
unsigned char CAN2_Tx_Message_Flag = 0;									//CAN2������Ϣ��־
/**
 *@Function:		CAN1_TX_IRQHandler(void)
 *@Description:	CAN1�����жϷ�����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN1_TX_IRQHandler(void)
{  
  if (CAN_GetITStatus (CAN1,CAN_IT_TME)!=RESET)                             
	{
		CAN1_Tx_Message_Flag=1;
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);    
	}
}


/**
 *@Function:		CAN2_TX_IRQHandler(void)
 *@Description:	CAN2�����жϷ�����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN2_TX_IRQHandler(void)
{  
  if (CAN_GetITStatus (CAN2,CAN_IT_TME)!=RESET)                            
	{
		CAN2_Tx_Message_Flag=1;
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);    
	}
}


/**
 *@Function:		CAN_Data_Decode(CAN_Data_TypeDef *CAN_Data,CanRxMsg *CAN_Rx_Message)
 *@Description:	CAN���ݽ���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN_Data_Decode(CAN_Data_TypeDef *CAN_Data,CanRxMsg *CAN_Rx_Message)
{
	CAN_Data->Origin_MechAngle 				= (CAN_Rx_Message->Data[0]<<8)|(CAN_Rx_Message->Data[1]);
	CAN_Data->Current_Speed 					= (CAN_Rx_Message->Data[2]<<8)|(CAN_Rx_Message->Data[3]);
	CAN_Data->Current								  = (CAN_Rx_Message->Data[4]<<8)|(CAN_Rx_Message->Data[5]);	
	CAN_Data->Temperature					    = (CAN_Rx_Message->Data[6]);
	
	
	if(CAN_Data->Critical_MechAngle !=0)//��ǰ����Ϊ��̨���ʹ�ã�����Ƕ����ݲ�ת���Ƕȷ�Χ
	{
		if(CAN_Data->Origin_MechAngle < CAN_Data->Critical_MechAngle)
		{
			CAN_Data->Current_MechAngle = CAN_Data->Origin_MechAngle + 8192;
		}
		else
		{
			CAN_Data->Current_MechAngle = CAN_Data->Origin_MechAngle;
		}		
	}
	else																//��ͨ���ʹ��ԭʼ�ĽǶ�ֵ
	{		
			CAN_Data->Current_MechAngle = CAN_Data->Origin_MechAngle;		
	}
}
//Ȧ����¼
void CAN_Total_Angle_Decode(CAN_Data_TypeDef *CAN_Data)
{
	static int Init_Flag = 0;
	static int Total_Num = 0;
	static int Init_Angle = 0;

	if(!Init_Flag)
	{
		Init_Angle = CAN_Data->Current_MechAngle; //������ʼ�Ƕȵ����
		Init_Flag = 1;
	}
	//��Ȧ��
	if((CAN_Data->Current_MechAngle - CAN_Data->Last_MechAngle) > 4095)
	{	
		Total_Num -= 1;
	}
	else if((CAN_Data->Current_MechAngle - CAN_Data->Last_MechAngle) < -4095)
	{	
		Total_Num += 1;		
	}	
	
	CAN_Data->Total_MechAngle = Total_Num*8192 + (CAN_Data->Current_MechAngle) - Init_Angle; //��һ�β����϶�����ת����ֻ�ü�ȥ��ʼ��
	CAN_Data->Last_MechAngle = CAN_Data->Current_MechAngle;	

}

static float bit8TObit32(uint8_t *change_info)
{
	union
	{
    float f;
		char  byte[4];
	}u32val;
    u32val.byte[0]=change_info[0];
    u32val.byte[1]=change_info[1];
    u32val.byte[2]=change_info[2];
    u32val.byte[3]=change_info[3];
	return u32val.f;
} 
/**
 *@Function:		CAN1_RX0_IRQHandler(void)            
 *@Description:	CAN1�����жϷ�����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN1_RX0_IRQHandler(void)                                             
{   
	CanRxMsg CAN1_Rx_Message;	
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN1,CAN_FIFO0,&CAN1_Rx_Message);
		if ( (CAN1_Rx_Message.IDE == CAN_Id_Standard) && (CAN1_Rx_Message.RTR == CAN_RTR_Data) && (CAN1_Rx_Message.DLC == 8) )//��׼֡������֡�����ݳ���Ϊ8�ֽ�
		{
			//CAN1�ź������
			CAN1_Signal = 50;
			switch (CAN1_Rx_Message.StdId)
			{
				//����
				case 0x201:
				case 0x202:
				case 0x203:
				case 0x204:
				{
					CAN_Data_Decode(&CAN_Chassis[CAN1_Rx_Message.StdId - 0x201],&CAN1_Rx_Message);
				}break;
				
				//Yaw
				case 0x205:
				{
					CAN_Data_Decode(&CAN_Gimbal[0],&CAN1_Rx_Message);
				}break;

				//Pitch
				case 0x209:
				{
					CAN_Data_Decode(&CAN_Gimbal[1],&CAN1_Rx_Message);
				}break;		
				
				//SuperCap�����п��ƣ�
				case 0x211:
				{
					//�������ݷ�������
					//�����ѹ
					//���ݵ�ѹ
					//�������
					//�趨����
					uint16_t* pPowerData = (uint16_t*)&CAN1_Rx_Message.Data;
					
					SuperCap_Info.InputVot=      (float) pPowerData[0] / 100.0f; //�����ѹ
					SuperCap_Info.CapVot = 			 (float) pPowerData[1] / 100.0f; //���ݵ�ѹ
					SuperCap_Info.InputCurrent = (float) pPowerData[2] / 100.0f; //�������
					SuperCap_Info.CurrentPower = (float) pPowerData[3] / 100.0f; //�趨����
					SuperCap_Info.Low_Filter_Vot = 0.3*SuperCap_Info.CapVot + (1-0.3)*SuperCap_Info.Low_Filter_Vot; //�������ݵ�ѹ��ͨ�˲�ֵ,�˲�ϵ��0.3
					SuperCap_Info.id             = 0x211;
					//���������ź�������
					SuperCap_Signal = 1000;
				}break;		
	
				//SuperCap�����ƣ�
				case 0x300:
				{
					//�������ݷ�������
					//�����ѹ
					//�������
					SuperCap_Info.CapVot = 			 bit8TObit32(CAN1_Rx_Message.Data); //���ݵ�ѹ
					SuperCap_Info.InputCurrent = bit8TObit32(&CAN1_Rx_Message.Data[4]); //�������
					SuperCap_Info.id             = 0x300;
				
					//���������ź�������
					SuperCap_Signal = 1000;
				}break;
				
				
				default:
				{
					break;
				}   
			}
		}
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		
	}
}


/**
 *@Function:		CAN2_RX0_IRQHandler(void)            
 *@Description:	CAN2�����жϷ�����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN2_RX0_IRQHandler(void)                                                 
{   
	CanRxMsg CAN2_Rx_Message;	
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN2,CAN_FIFO0,&CAN2_Rx_Message);

		if ( (CAN2_Rx_Message.IDE == CAN_Id_Standard) && (CAN2_Rx_Message.RTR == CAN_RTR_Data) && (CAN2_Rx_Message.DLC == 8) )//��׼֡������֡�����ݳ���Ϊ8�ֽ�
		{
			//CAN2�ź������
			CAN2_Signal = 50;
			switch (CAN2_Rx_Message.StdId)
			{
				//�����̵��
				case 0x202:
				{
					CAN_Data_Decode(&CAN_Trigger,&CAN2_Rx_Message);
					CAN_Total_Angle_Decode(&CAN_Trigger);
				}break;
				
				//Ħ������
				case 0x203:
				{
					CAN_Data_Decode(&CAN_Shoot[0],&CAN2_Rx_Message);
				}break;
				
				//Ħ������
				case 0x204:
				{
					CAN_Data_Decode(&CAN_Shoot[1],&CAN2_Rx_Message);
				}break;
				
				//����
				case 0x205:
				case 0x206:
				case 0x207:
				case 0x208:
				{
					CAN_Data_Decode(&CAN_Rudder[CAN2_Rx_Message.StdId - 0x205],&CAN2_Rx_Message);
				}break;				
				
				
				default:
				{
					break; 
				}
					
			}
		}
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);		
	}
}

/**
 *@Function:		CAN1_TX_Chassis(void)
 *@Description:	CAN1�������ݷ���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN1_TX_Chassis(void)
{
	CanTxMsg CAN1_Tx_Message;

	CAN1_Tx_Message.IDE = CAN_ID_STD;                                               //��׼֡
	CAN1_Tx_Message.RTR = CAN_RTR_DATA;                                             //����֡
	CAN1_Tx_Message.DLC = 0x08;                                                     //֡����Ϊ8
	CAN1_Tx_Message.StdId = 0x200;                               										//֡IDΪ���������CAN_ID
	
	CAN1_Tx_Message.Data[0] = (CAN_Chassis[0].Target_Current>>8)&0xff;             //201���յ�����8λ
	CAN1_Tx_Message.Data[1] = (CAN_Chassis[0].Target_Current)&0xff;                 //201���յ�����8λ
	CAN1_Tx_Message.Data[2] = (CAN_Chassis[1].Target_Current>>8)&0xff;             //202���յ�����8λ
	CAN1_Tx_Message.Data[3] = (CAN_Chassis[1].Target_Current)&0xff;                //202���յ�����8λ
	CAN1_Tx_Message.Data[4] = (CAN_Chassis[2].Target_Current>>8)&0xff;             //203���յ�����8λ
	CAN1_Tx_Message.Data[5] = (CAN_Chassis[2].Target_Current)&0xff;                 //203���յ�����8λ
	CAN1_Tx_Message.Data[6] = (CAN_Chassis[3].Target_Current>>8)&0xff;             //204���յ�����8λ
	CAN1_Tx_Message.Data[7] = (CAN_Chassis[3].Target_Current)&0xff;                 //204���յ�����8λ

	CAN1_Tx_Message_Flag = 0;
	
	//CAN1���ߴ���
	if(CAN1_Signal > 0)
	{
	CAN_Transmit(CAN1,&CAN1_Tx_Message);	
	}
	
	while(CAN1_Tx_Message_Flag == 0)
	{
		if(CAN1_Signal <= 0)
		{
			break;
		}
	}
}
/**
 *@Function:		CAN1_TX_Gimbal_Pitch(void)
 *@Description:	CAN1��̨Pitch���ݷ���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN1_TX_Gimbal_Pitch(void)
{
	CanTxMsg CAN1_Tx_Message;

	CAN1_Tx_Message.IDE = CAN_ID_STD;                                               //��׼֡
	CAN1_Tx_Message.RTR = CAN_RTR_DATA;                                             //����֡
	CAN1_Tx_Message.DLC = 0x08;                                                     //֡����Ϊ8
	CAN1_Tx_Message.StdId = 0x2FF;                               //֡IDΪ���������CAN_ID

	CAN1_Tx_Message.Data[0] = (CAN_Gimbal[1].Target_Current>>8)&0xff;             //201���յ�����8λ
	CAN1_Tx_Message.Data[1] = (CAN_Gimbal[1].Target_Current)&0xff;                 //201���յ�����8λ
	CAN1_Tx_Message.Data[2] = 0;           
	CAN1_Tx_Message.Data[3] = 0;             
	CAN1_Tx_Message.Data[4] = 0;            
	CAN1_Tx_Message.Data[5] = 0;               
	CAN1_Tx_Message.Data[6] = 0;            
	CAN1_Tx_Message.Data[7] = 0;              

	CAN1_Tx_Message_Flag = 0;
	
	//CAN1���ߴ���
	if(CAN1_Signal > 0)
	{
	CAN_Transmit(CAN1,&CAN1_Tx_Message);	
	}
	
	while(CAN1_Tx_Message_Flag == 0)
	{
		if(CAN1_Signal <= 0)
		{
			break;
		}
	}
}

/**
 *@Function:		CAN1_TX_Gimbal_Yaw(void)
 *@Description:	CAN1��̨Yaw���ݷ���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN1_TX_Gimbal_Yaw(void)
{
	CanTxMsg CAN1_Tx_Message;

	CAN1_Tx_Message.IDE = CAN_ID_STD;                                               //��׼֡
	CAN1_Tx_Message.RTR = CAN_RTR_DATA;                                             //����֡
	CAN1_Tx_Message.DLC = 0x08;                                                     //֡����Ϊ8
	CAN1_Tx_Message.StdId = 0x1FF;                               //֡IDΪ���������CAN_ID

	CAN1_Tx_Message.Data[0] = (CAN_Gimbal[0].Target_Current>>8)&0xff;             //201���յ�����8λ
	CAN1_Tx_Message.Data[1] = (CAN_Gimbal[0].Target_Current)&0xff;                 //201���յ�����8λ
	CAN1_Tx_Message.Data[2] = 0;           
	CAN1_Tx_Message.Data[3] = 0;             
	CAN1_Tx_Message.Data[4] = 0;            
	CAN1_Tx_Message.Data[5] = 0;               
	CAN1_Tx_Message.Data[6] = 0;            
	CAN1_Tx_Message.Data[7] = 0;              

	CAN1_Tx_Message_Flag = 0;
	
	//CAN1���ߴ���
	if(CAN1_Signal > 0)
	{
	CAN_Transmit(CAN1,&CAN1_Tx_Message);	
	}
	
	while(CAN1_Tx_Message_Flag == 0)
	{
		if(CAN1_Signal <= 0)
		{
			break;
		}
	}
	
}


/**
 *@Function:		CAN2_TX_Shoot(void)
 *@Description:	CAN2�������ݷ���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN2_TX_Shoot(void)
{
	CanTxMsg CAN2_Tx_Message;

	CAN2_Tx_Message.IDE = CAN_ID_STD;                                               //��׼֡
	CAN2_Tx_Message.RTR = CAN_RTR_DATA;                                             //����֡
	CAN2_Tx_Message.DLC = 0x08;                                                     //֡����Ϊ8
	CAN2_Tx_Message.StdId = 0x200;                               //֡IDΪ���������CAN_ID
	
	CAN2_Tx_Message.Data[0] = 0;  
	CAN2_Tx_Message.Data[1] = 0;      
	CAN2_Tx_Message.Data[2] = (CAN_Trigger.Target_Current>>8)&0xff;           
	CAN2_Tx_Message.Data[3] = (CAN_Trigger.Target_Current)&0xff;           
	CAN2_Tx_Message.Data[4] = (CAN_Shoot[0].Target_Current>>8)&0xff;           
	CAN2_Tx_Message.Data[5] = (CAN_Shoot[0].Target_Current)&0xff;             
	CAN2_Tx_Message.Data[6] = (CAN_Shoot[1].Target_Current>>8)&0xff;               
	CAN2_Tx_Message.Data[7] = (CAN_Shoot[1].Target_Current)&0xff;        

	CAN2_Tx_Message_Flag = 0;
	
	//CAN2���ߴ���
	if(CAN2_Signal > 0)
	{
		CAN_Transmit(CAN2,&CAN2_Tx_Message);
	}
	
	while(CAN2_Tx_Message_Flag == 0)
	{
		if(CAN2_Signal <= 0)
		{
			break;
		}
	}
}

/**
 *@Function:		CAN2_TX_Rudder(void)
 *@Description:	CAN2�����ݷ���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN2_TX_Rudder(void)
{
	CanTxMsg CAN2_Tx_Message;

	CAN2_Tx_Message.IDE = CAN_ID_STD;                                               //��׼֡
	CAN2_Tx_Message.RTR = CAN_RTR_DATA;                                             //����֡
	CAN2_Tx_Message.DLC = 0x08;                                                     //֡����Ϊ8
	CAN2_Tx_Message.StdId = 0x1FF;                              									  //֡IDΪ���������CAN_ID
	
	CAN2_Tx_Message.Data[0] = (CAN_Rudder[0].Target_Current>>8)&0xff;  
	CAN2_Tx_Message.Data[1] = (CAN_Rudder[0].Target_Current)&0xff;       
	CAN2_Tx_Message.Data[2] = (CAN_Rudder[1].Target_Current>>8)&0xff;  
	CAN2_Tx_Message.Data[3] = (CAN_Rudder[1].Target_Current)&0xff;                
	CAN2_Tx_Message.Data[4] = (CAN_Rudder[2].Target_Current>>8)&0xff;  
	CAN2_Tx_Message.Data[5] = (CAN_Rudder[2].Target_Current)&0xff;                   
	CAN2_Tx_Message.Data[6] = (CAN_Rudder[3].Target_Current>>8)&0xff;             
	CAN2_Tx_Message.Data[7] = (CAN_Rudder[3].Target_Current)&0xff;     

	//
//	CAN2_Tx_Message.Data[0] = 0;  
//	CAN2_Tx_Message.Data[1] = 0; 
//	CAN2_Tx_Message.Data[2] = 0;  
//	CAN2_Tx_Message.Data[3] = 0;                
//	CAN2_Tx_Message.Data[4] = 0;  
//	CAN2_Tx_Message.Data[5] = 0;                   
//	CAN2_Tx_Message.Data[6] = 0;             
//	CAN2_Tx_Message.Data[7] = 0;  
	
	CAN2_Tx_Message_Flag = 0;
	
	//CAN2���ߴ���
	if(CAN2_Signal > 0)
	{
		CAN_Transmit(CAN2,&CAN2_Tx_Message);
	}
	
	while(CAN2_Tx_Message_Flag == 0)
	{
		if(CAN2_Signal <= 0)
		{
			break;
		}
	}
}

int power_relay = 0;
int power_flag = 0;

/**
 *@Function:		CAN1_TX_SuperCap(void)
 *@Description:	CAN1�����������ݷ���
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void CAN1_TX_SuperCap(void)
{
	CanTxMsg CAN1_Tx_Message;
	
	if(SuperCap_Info.id == 0x210)
	{
		//SuperCap�����п��ƣ�
		CAN1_Tx_Message.IDE = CAN_ID_STD;                                               //��׼֡
		CAN1_Tx_Message.RTR = CAN_RTR_DATA;                                             //����֡
		CAN1_Tx_Message.DLC = 0x08;                                                     //֡����Ϊ8
		CAN1_Tx_Message.StdId = 0x210;                              									  //֡IDΪ���������CAN_ID

		CAN1_Tx_Message.Data[0] = ((game_robot_state.chassis_power_limit*100)>>8)&0xff; 
		CAN1_Tx_Message.Data[1] = (game_robot_state.chassis_power_limit*100)&0xff; 
		CAN1_Tx_Message.Data[2] = 0;           
		CAN1_Tx_Message.Data[3] = 0;             
		CAN1_Tx_Message.Data[4] = 0;            
		CAN1_Tx_Message.Data[5] = 0;               
		CAN1_Tx_Message.Data[6] = 0;            
		CAN1_Tx_Message.Data[7] = 0;   		
	}
	else
	{
		//���ݵ��̹����趨ϵ��
		switch(game_robot_state.chassis_power_limit)
		{
			case 45:
				power_flag = 1;
				break;
			case 50:
				power_flag = 2;
				break;
			case 55:
				power_flag = 3;
				break;
			case 60:
				power_flag = 4;
				break;
			case 80:
				power_flag = 5;
				break;		
			case 100:
				power_flag = 6;
				break;
			default:
				power_flag = 1;
				break;		
		}
		
		
		//SuperCap�����ƣ�	
		CAN1_Tx_Message.IDE = CAN_ID_STD;                                               //��׼֡
		CAN1_Tx_Message.RTR = CAN_RTR_DATA;                                             //����֡
		CAN1_Tx_Message.DLC = 0x08;                                                     //֡����Ϊ8
		CAN1_Tx_Message.StdId = 0x427;  

		CAN1_Tx_Message.Data[0] = power_relay; 
		CAN1_Tx_Message.Data[1] = power_flag; 
		CAN1_Tx_Message.Data[2] = game_robot_state.chassis_power_limit-power_heat_data.chassis_power;           
		CAN1_Tx_Message.Data[3] = 0;             
		CAN1_Tx_Message.Data[4] = 0;            
		CAN1_Tx_Message.Data[5] = 0;               
		CAN1_Tx_Message.Data[6] = 0;            
		CAN1_Tx_Message.Data[7] = 0;   		
	}

	CAN1_Tx_Message_Flag = 0;
	
	//CAN1���ߴ���
	if(CAN1_Signal > 0)
	{
	CAN_Transmit(CAN1,&CAN1_Tx_Message);	
	}
	
	while(CAN1_Tx_Message_Flag == 0)
	{
		if(CAN1_Signal <= 0)
		{
			break;
		}
	}
}