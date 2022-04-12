	/**
  ****************************(C) COPYRIGHT 2021 SHUIMUJIEMING****************************
  * @file       custom_ui.c/h
  * @brief      �Զ���UI������ʾ��غ���
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     7-10-2021     shuimujieming              1. ���
  *
  ****************************(C) COPYRIGHT 2021 SHUIMUJIEMING****************************
  */
#include "custom_ui.h"


//���巢���ߺͽ�����ID��Ĭ��Ϊ����
int Sendor_ID ;
int Receiver_ID ;


//��������������
ext_client_custom_graphic_int_t interger_one;

//���帡��������
ext_client_custom_graphic_float_t float_one;

//�����ַ�����
ext_client_custom_char_int_t character_one;
ext_client_custom_char_int_t character_two;
ext_client_custom_char_int_t character_three;
ext_client_custom_char_int_t character_four;
ext_client_custom_char_int_t character_five;
ext_client_custom_char_int_t character_six;


//����ֱ������
ext_student_custom_graphic_t line_one;
ext_student_custom_graphic_t line_two;
ext_student_custom_graphic_t line_three;
ext_student_custom_graphic_t line_four;
ext_student_custom_graphic_t line_five;
ext_student_custom_graphic_t line_six;
ext_student_custom_graphic_t line_seven;
ext_student_custom_graphic_t line_eight;
ext_student_custom_graphic_t line_nine;


//����Բ����
ext_student_custom_graphic_t circle_one;

//�����ַ�������
char char_one[30];	//��������

char char_two[30];	//��̨ģʽ

char char_three[30];	//���������λ

char char_four[30];	//Pitch

char char_five[30];	//Ħ���ֿ���

char char_six[30];	//����ģʽ

/**
  * @brief 	����ֱ��
  * @param  Graphic ͼ������
						Graphic_Name ͼ����
						Operate_Type	��������
						Graphic_Layer ͼ��
						Graphic_Color ͼ����ɫ
						Graphic_Width	�߿�
						Start_x��Start_y ��ʼ����
						End_x��End_y ��������
  * @retval 
  * @notes  
  */
void Draw_Line(ext_student_custom_graphic_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,u32 Graphic_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//ͼ����
	for(i=0;i<3&&Graphic_Name[i]!='\0';i++)
	Graphic->grapic_data_struct.graphic_name[i]=Graphic_Name[i];
	
	Graphic->grapic_data_struct.graphic_tpye = Graphic_Type_LINE;
	Graphic->grapic_data_struct.operate_tpye = Operate_Type;	
	Graphic->grapic_data_struct.layer = Graphic_Layer;
	Graphic->grapic_data_struct.color = Graphic_Color;

	Graphic->grapic_data_struct.width = Graphic_Width;
	Graphic->grapic_data_struct.start_x = Start_x;
	Graphic->grapic_data_struct.start_y = Start_y;
	
	Graphic->grapic_data_struct.end_x = End_x;
	Graphic->grapic_data_struct.end_y = End_y;

}
	
/**
  * @brief 	����Բ
  * @param  Graphic ͼ������
						Graphic_Name ͼ����
						Operate_Type	��������
						Graphic_Layer ͼ��
						Graphic_Color ͼ����ɫ
						Graphic_Width	�߿�
						Start_x��Start_y Բ������
						Radius �뾶
  * @retval 
  * @notes  
  */
void Draw_Circle(ext_student_custom_graphic_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,u32 Graphic_Width,u32 Start_x,u32 Start_y,u32 Radius)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//ͼ����
	for(i=0;i<3&&Graphic_Name[i]!='\0';i++)
	Graphic->grapic_data_struct.graphic_name[i]=Graphic_Name[i];
	
	Graphic->grapic_data_struct.graphic_tpye = Graphic_Type_CIRCLE;
	Graphic->grapic_data_struct.operate_tpye = Operate_Type;	
	Graphic->grapic_data_struct.layer = Graphic_Layer;
	Graphic->grapic_data_struct.color = Graphic_Color;

	Graphic->grapic_data_struct.width = Graphic_Width;
	Graphic->grapic_data_struct.start_x = Start_x;
	Graphic->grapic_data_struct.start_y = Start_y;
	
	Graphic->grapic_data_struct.radius = Radius;


}

	/**
  * @brief 	����ֱ��
  * @param  Graphic ͼ������
						Graphic_Name ͼ����
						Operate_Type	��������
						Graphic_Layer ͼ��
						Graphic_Color ͼ����ɫ
						Graphic_Width	�߿�
						Start_x��Start_y ��ʼ����
						End_x��End_y �ԽǶ�������
  * @retval 
  * @notes  
  */
void Draw_Rect(ext_student_custom_graphic_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,u32 Graphic_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//ͼ����
	for(i=0;i<3&&Graphic_Name[i]!='\0';i++)
	Graphic->grapic_data_struct.graphic_name[i]=Graphic_Name[i];
	
	Graphic->grapic_data_struct.graphic_tpye = Graphic_Type_RECT;
	Graphic->grapic_data_struct.operate_tpye = Operate_Type;	
	Graphic->grapic_data_struct.layer = Graphic_Layer;
	Graphic->grapic_data_struct.color = Graphic_Color;

	Graphic->grapic_data_struct.width = Graphic_Width;
	Graphic->grapic_data_struct.start_x = Start_x;
	Graphic->grapic_data_struct.start_y = Start_y;
	
	Graphic->grapic_data_struct.end_x = End_x;
	Graphic->grapic_data_struct.end_y = End_y;

}
	/**
  * @brief 	�����ַ�
  * @param  Graphic ͼ������
						Graphic_Name ͼ����
						Operate_Type	��������
						Graphic_Layer ͼ��
						Graphic_Color ͼ����ɫ
						Graphic_Width	�߿�
						Start_x��Start_y ��ʼ����
						End_x��End_y �ԽǶ�������
						Font_Size �����С
						Graphic_Char �ַ���
  * @retval 
  * @notes  
  */
void Draw_Char(ext_client_custom_char_int_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,char* Graphic_Char,u32 Font_Size,u32 Graphic_Width,u32 Start_x,u32 Start_y)
{
	int i;
	Graphic->data_cmd_id = 0x0110;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//ͼ����
	for(i=0;i<3&&Graphic_Name[i]!='\0';i++)
	Graphic->character_custom.grapic_data_struct.graphic_name[i]=Graphic_Name[i];
	
	Graphic->character_custom.grapic_data_struct.graphic_tpye = Graphic_Type_CHAR;
	Graphic->character_custom.grapic_data_struct.operate_tpye = Operate_Type;	
	Graphic->character_custom.grapic_data_struct.layer = Graphic_Layer;
	Graphic->character_custom.grapic_data_struct.color = Graphic_Color;

	Graphic->character_custom.grapic_data_struct.width = Graphic_Width;
	Graphic->character_custom.grapic_data_struct.start_x = Start_x;
	Graphic->character_custom.grapic_data_struct.start_y = Start_y;
	
	Graphic->character_custom.grapic_data_struct.start_angle = Font_Size;
	Graphic->character_custom.grapic_data_struct.end_angle = 30;

	for(int j=0;j<30;j++)
	{
	 Graphic->character_custom.data[j] = *Graphic_Char;		
	 Graphic_Char++;
	}

}


	/**
  * @brief 	��������
  * @param  Graphic ͼ������
						Graphic_Name ͼ����
						Operate_Type	��������
						Graphic_Layer ͼ��
						Graphic_Color ͼ����ɫ
						Graphic_Width	�߿�
						Start_x��Start_y ��ʼ����
						int_number 32λ������
  * @retval 
  * @notes  
  */
void Draw_Int(ext_client_custom_graphic_int_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,int int_number,u32 Font_Size,u32 Graphic_Width,u32 Start_x,u32 Start_y)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//ͼ����
	for(i=0;i<3&&Graphic_Name[i]!='\0';i++)
	Graphic->graphic_int_data_struct.graphic_name[i]=Graphic_Name[i];
	
	Graphic->graphic_int_data_struct.graphic_tpye = Graphic_Type_INT;
	Graphic->graphic_int_data_struct.operate_tpye = Operate_Type;	
	Graphic->graphic_int_data_struct.layer = Graphic_Layer;
	Graphic->graphic_int_data_struct.color = Graphic_Color;

	Graphic->graphic_int_data_struct.width = Graphic_Width;
	Graphic->graphic_int_data_struct.start_x = Start_x;
	Graphic->graphic_int_data_struct.start_y = Start_y;
	
	Graphic->graphic_int_data_struct.start_angle = Font_Size;
	Graphic->graphic_int_data_struct.int_number = int_number;
}


	/**
  * @brief 	���Ƹ�����
  * @param  Graphic ͼ������
						Graphic_Name ͼ����
						Operate_Type	��������
						Graphic_Layer ͼ��
						Graphic_Color ͼ����ɫ
						Graphic_Width	�߿�
						Start_x��Start_y ��ʼ����
						float_number	32λ������
						useful_digits С��λ��Ч����
  * @retval 
  * @notes  
  */
void Draw_Float(ext_client_custom_graphic_float_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,float float_number,int useful_digits,u32 Font_Size,u32 Graphic_Width,u32 Start_x,u32 Start_y)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//ͼ����
	for(i=0;i<3&&Graphic_Name[i]!='\0';i++)
	Graphic->graphic_float_data_struct.graphic_name[i]=Graphic_Name[i];
	
	Graphic->graphic_float_data_struct.graphic_tpye = Graphic_Type_FLOAT;
	Graphic->graphic_float_data_struct.operate_tpye = Operate_Type;	
	Graphic->graphic_float_data_struct.layer = Graphic_Layer;
	Graphic->graphic_float_data_struct.color = Graphic_Color;

	Graphic->graphic_float_data_struct.width = Graphic_Width;
	Graphic->graphic_float_data_struct.start_x = Start_x;
	Graphic->graphic_float_data_struct.start_y = Start_y;
	
	Graphic->graphic_float_data_struct.start_angle = Font_Size;
	Graphic->graphic_float_data_struct.end_angle = useful_digits;
	Graphic->graphic_float_data_struct.float_number = float_number;
}



////��̬�����������ļ����ã�������ϵͳ�����ͺ����ڴ��ض���
//static void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len)
//{
//	Referee_Data_Send(cmd_id,(uint8_t *)&p_data);
//}


char Send_Once_Flag = 0;//��ʼ����־
extern int Pitch_Angle_NUC;
extern Chassis_Mode_Enum Chassis_Mode;
extern Gimbal_Mode_Enum Gimbal_Mode;
extern Shoot_Mode_Enum Shoot_Mode;
//��ʾUI����ͼ��
void Show_UI()
{
	//������ID����
	if(game_robot_state.robot_id < 100)
	{
		Sendor_ID = game_robot_state.robot_id;
		Receiver_ID = 0x100 + game_robot_state.robot_id;
	}
	else if(game_robot_state.robot_id > 100)
	{
		Sendor_ID = game_robot_state.robot_id;
		Receiver_ID = 0x0164 + (game_robot_state.robot_id - 100);		
	}
	
	//�ڴ����
	memset(char_one,'\0',sizeof(char_one));
	memset(char_two,'\0',sizeof(char_two));
	memset(char_three,'\0',sizeof(char_three));
	memset(char_four,'\0',sizeof(char_four));
	memset(char_five,'\0',sizeof(char_five));
	memset(char_six,'\0',sizeof(char_six));


	//ֻ��Ҫ����һ�ε����ݣ������ͼ�β���
	if(Send_Once_Flag == 0)
	{
		
	Send_Once_Flag = 1;//��־λ��1,���ٽ�����Ӳ���
		
	//�ڴ������ÿ�
	//������д���ַ���������ͬprintf������
	sprintf(char_one,"Super Cap %.2f",100.0);
	//���ַ�ͼ�����ݳ�ʼ��
	Draw_Char(&character_one,"001",Graphic_Operate_ADD,9,Graphic_Color_Yellow,char_one,12,1,80,780);
	
	//ͬ��
	sprintf(char_two,"Gimbal Mode is Rotate %d",1);
	Draw_Char(&character_two,"100",Graphic_Operate_ADD,9,Graphic_Color_Yellow,char_two,12,1,80,800);
	
	sprintf(char_three,"Shoot Limit Wait");
	Draw_Char(&character_three,"101",Graphic_Operate_ADD,9,Graphic_Color_Yellow,char_three,12,1,80,820);		

	sprintf(char_four,"Pitch %.2f",imu.pit / 100.0f);
	Draw_Char(&character_four,"102",Graphic_Operate_ADD,9,Graphic_Color_Yellow,char_three,12,1,80,840);		
			
	sprintf(char_five,"Fric status off");
	Draw_Char(&character_five,"103",Graphic_Operate_ADD,9,Graphic_Color_Yellow,char_five,12,1,80,860);		
	
	sprintf(char_six,"Aim Mode Robot");
	Draw_Char(&character_six,"104",Graphic_Operate_ADD,9,Graphic_Color_Yellow,char_six,12,1,80,880);		
	
		//��ʼ����������
	Draw_Int(&interger_one,"301",Graphic_Operate_ADD,9,Graphic_Color_Yellow,power_heat_data.shooter_id1_42mm_cooling_heat,12,1,80,820);
	
	//��ʼ������������
	Draw_Float(&float_one,"401",Graphic_Operate_ADD,9,Graphic_Color_Yellow,real_shoot_data.bullet_speed,3,12,1,80,840);
	
	//��ʼ��ֱ������
	Draw_Line(&line_one,"002",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2,SCREEN_WIDTH/2,SCREEN_LENGTH/2,SCREEN_WIDTH/2-500);
	Draw_Line(&line_two,"003",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-100,SCREEN_WIDTH/2,SCREEN_LENGTH/2+100,SCREEN_WIDTH/2);
	Draw_Line(&line_three,"004",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-80,SCREEN_WIDTH/2-80,SCREEN_LENGTH/2+80,SCREEN_WIDTH/2-80);
	Draw_Line(&line_four,"005",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-60,SCREEN_WIDTH/2-160,SCREEN_LENGTH/2+60,SCREEN_WIDTH/2-160);
	Draw_Line(&line_five,"006",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-40,SCREEN_WIDTH/2-220,SCREEN_LENGTH/2+40,SCREEN_WIDTH/2-220);

	Draw_Line(&line_six,"007",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-40,SCREEN_WIDTH/2,SCREEN_LENGTH/2-40,SCREEN_WIDTH/2-210);
	Draw_Line(&line_seven,"008",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2+40,SCREEN_WIDTH/2,SCREEN_LENGTH/2+40,SCREEN_WIDTH/2-210);

	Draw_Line(&line_eight,"009",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-90,SCREEN_WIDTH/2-40,SCREEN_LENGTH/2+90,SCREEN_WIDTH/2-40);
	Draw_Line(&line_nine,"010",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-70,SCREEN_WIDTH/2-120,SCREEN_LENGTH/2+70,SCREEN_WIDTH/2-120);

	//��ʼ��Բ������
	Draw_Circle(&circle_one,"200",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2+60,SCREEN_WIDTH/2-160,20);

	//�����ϵͳ���ڷ���ͼ������
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_one,sizeof(character_one));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_two,sizeof(character_two));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_three,sizeof(character_three));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_four,sizeof(character_four));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_five,sizeof(character_five));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_six,sizeof(character_six));


	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&line_one,sizeof(line_one));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&line_two,sizeof(line_two));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&line_three,sizeof(line_three));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&line_four,sizeof(line_four));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&line_five,sizeof(line_five));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&line_six,sizeof(line_six));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&line_seven,sizeof(line_seven));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&line_eight,sizeof(line_eight));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&line_nine,sizeof(line_nine));

	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&circle_one,sizeof(circle_one));
	


	}	
	//��Ҫ���ϸ��µ����ݷ���
	else
	{
//	if(chassis_speed_mode == CHASSIS_SUPER_SPEED_MODE)
	{sprintf(char_one,"Super Cap %.2f ON",SuperCap_Info.CapVot);}
	
//	else if(chassis_speed_mode != CHASSIS_SUPER_SPEED_MODE)
	{sprintf(char_one,"Super Cap %.2f OFF",SuperCap_Info.CapVot);}

	if(SuperCap_Info.CapVot < 19.0)
	{Draw_Char(&character_one,"001",Graphic_Operate_CHANGE,9,Graphic_Color_Purplish_red,char_one,12,1,80,780);		}
	else
	{Draw_Char(&character_one,"001",Graphic_Operate_CHANGE,9,Graphic_Color_Green,char_one,12,1,80,780);				}
	
		 
	if(Gimbal_Mode == GIMBAL_MODE_FOLLOW)
	{sprintf(char_two,"Gimbal Mode is Follow");}
	else	if(Gimbal_Mode == GIMBAL_MODE_ROTATE)
	{sprintf(char_two,"Gimbal Mode is Rotate");}

	Draw_Char(&character_two,"100",Graphic_Operate_CHANGE,9,Graphic_Color_Yellow,char_two,12,1,80,800);

	sprintf(char_four,"Pitch %.2f",Pitch_Angle_NUC / 100.0f);
	Draw_Char(&character_four,"102",Graphic_Operate_CHANGE,9,Graphic_Color_Yellow,char_four,12,1,80,840);		
	
	if(Shoot_Mode == SHOOT_MODE_RUN)//Ħ���ֿ���ģʽ��
	{
	sprintf(char_five,"Fric status on   ");
	if(abs(CAN_Shoot[0].Current_Speed) < 10)//Ħ���ֶ�ס��
	{
	sprintf(char_five,"Fric status block");	
	}
		Draw_Char(&character_five,"103",Graphic_Operate_CHANGE,9,Graphic_Color_Yellow,char_five,12,1,80,860);				
	}
	else if(Shoot_Mode == SHOOT_MODE_STOP)//Ħ���ֹر�
	{
	sprintf(char_five,"Fric status off  ");
	Draw_Char(&character_five,"103",Graphic_Operate_CHANGE,9,Graphic_Color_Yellow,char_five,12,1,80,860);						
	}

	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_one,sizeof(character_one));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_two,sizeof(character_two));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_three,sizeof(character_three));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_four,sizeof(character_four));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_five,sizeof(character_five));
	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&character_six,sizeof(character_six));

	}
}