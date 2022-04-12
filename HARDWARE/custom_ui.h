#ifndef __CUSTOM_UI_H
#define __CUSTOM_UI_H
#include "main.h"


//��Ļ���
#define SCREEN_WIDTH 1080
//��Ļ����
#define SCREEN_LENGTH 1920

//ͼ�β���ѡ��
#define Graphic_Operate_ADD 1
#define Graphic_Operate_CHANGE 2
#define Graphic_Operate_DEL 3

//ͼ������
#define Graphic_Type_LINE 0
#define Graphic_Type_RECT 1
#define Graphic_Type_CIRCLE 2
#define Graphic_Type_FLOAT 5
#define Graphic_Type_INT 6
#define Graphic_Type_CHAR 7

//ͼ����ɫ
#define Graphic_Color_Main 0         //������ɫ
#define Graphic_Color_Yellow 1			 //��ɫ
#define Graphic_Color_Green 2				 //��ɫ
#define Graphic_Color_Orange 3			 //��ɫ
#define Graphic_Color_Purplish_red 4 //�Ϻ�ɫ
#define Graphic_Color_Pink 5				 //��ɫ
#define Graphic_Color_Cyan 6         //��ɫ
#define Graphic_Color_Black 7				 //��ɫ
#define Graphic_Color_White 8				 //��ɫ


//�ͻ���ɾ��ͼ�� 0x0100
typedef __packed struct
{
uint8_t operate_tpye;
uint8_t layer;
}ext_client_custom_graphic_delete_t;

//ͼ����������
typedef __packed struct
{
uint8_t graphic_name[3];
uint32_t operate_tpye:3;
uint32_t graphic_tpye:3;
uint32_t layer:4;
uint32_t color:4;
uint32_t start_angle:9;
uint32_t end_angle:9;
uint32_t width:10;
uint32_t start_x:11;
uint32_t start_y:11;
uint32_t radius:10;
uint32_t end_x:11;
uint32_t end_y:11;
}graphic_data_struct_t;
//��������
typedef __packed struct
{
uint8_t graphic_name[3];
uint32_t operate_tpye:3;
uint32_t graphic_tpye:3;
uint32_t layer:4;
uint32_t color:4;
uint32_t start_angle:9;
uint32_t end_angle:9;
uint32_t width:10;
uint32_t start_x:11;
uint32_t start_y:11;
int32_t	 int_number;
}graphic_int_data_struct_t;
//����������
typedef __packed struct
{
uint8_t graphic_name[3];
uint32_t operate_tpye:3;
uint32_t graphic_tpye:3;
uint32_t layer:4;
uint32_t color:4;
uint32_t start_angle:9;
uint32_t end_angle:9;
uint32_t width:10;
uint32_t start_x:11;
uint32_t start_y:11;
float	 float_number;
}graphic_float_data_struct_t;
//�ַ�����
typedef __packed struct
{
graphic_data_struct_t grapic_data_struct;
uint8_t data[30];
}graphic_char_data_struct_t;
//�ͻ��˻���ͼ��
typedef __packed struct
{
uint16_t data_cmd_id;	//���ݶ�����ID
uint16_t sender_ID;	//������ID
uint16_t receiver_ID;	//������ID
graphic_data_struct_t grapic_data_struct;//ͼ������
}ext_student_custom_graphic_t;	

//�ͻ��˻�������
typedef __packed struct
{
uint16_t data_cmd_id;	//���ݶ�����ID
uint16_t sender_ID;	//������ID
uint16_t receiver_ID;	//������ID
graphic_int_data_struct_t graphic_int_data_struct;//��������
}ext_client_custom_graphic_int_t;

//�ͻ��˻��Ƹ�����
typedef __packed struct
{
uint16_t data_cmd_id;	//���ݶ�����ID
uint16_t sender_ID;	//������ID
uint16_t receiver_ID;	//������ID
graphic_float_data_struct_t graphic_float_data_struct;//����������
}ext_client_custom_graphic_float_t;

//�ͻ��˻����ַ�
typedef __packed struct
{
uint16_t data_cmd_id;	//���ݶ�����ID
uint16_t sender_ID;	//������ID
uint16_t receiver_ID;	//������ID
graphic_char_data_struct_t character_custom;//�ַ�����
}ext_client_custom_char_int_t;	

extern char Send_Once_Flag;//��ʼ����־

void Draw_Line(ext_student_custom_graphic_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,u32 Graphic_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y);
void Draw_Circle(ext_student_custom_graphic_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,u32 Graphic_Width,u32 Start_x,u32 Start_y,u32 Radius);
void Draw_Rect(ext_student_custom_graphic_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,u32 Graphic_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y);
void Draw_Char(ext_client_custom_char_int_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,char* Graphic_Char,u32 Font_Size,u32 Graphic_Width,u32 Start_x,u32 Start_y);
void Show_UI();
#endif