	/**
  ****************************(C) COPYRIGHT 2021 SHUIMUJIEMING****************************
  * @file       custom_ui.c/h
  * @brief      自定义UI界面显示相关函数
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     7-10-2021     shuimujieming              1. 完成
  *
  ****************************(C) COPYRIGHT 2021 SHUIMUJIEMING****************************
  */
#include "custom_ui.h"


//定义发送者和接收者ID，默认为蓝方
int Sendor_ID ;
int Receiver_ID ;


//定义整数型数据
ext_client_custom_graphic_int_t interger_one;

//定义浮点型数据
ext_client_custom_graphic_float_t float_one;

//定义字符数据
ext_client_custom_char_int_t character_one;
ext_client_custom_char_int_t character_two;
ext_client_custom_char_int_t character_three;
ext_client_custom_char_int_t character_four;
ext_client_custom_char_int_t character_five;
ext_client_custom_char_int_t character_six;


//定义直线数据
ext_student_custom_graphic_t line_one;
ext_student_custom_graphic_t line_two;
ext_student_custom_graphic_t line_three;
ext_student_custom_graphic_t line_four;
ext_student_custom_graphic_t line_five;
ext_student_custom_graphic_t line_six;
ext_student_custom_graphic_t line_seven;
ext_student_custom_graphic_t line_eight;
ext_student_custom_graphic_t line_nine;


//定义圆数据
ext_student_custom_graphic_t circle_one;

//定义字符串变量
char char_one[30];	//超级电容

char char_two[30];	//云台模式

char char_three[30];	//射击允许限位

char char_four[30];	//Pitch

char char_five[30];	//摩擦轮开关

char char_six[30];	//自瞄模式

/**
  * @brief 	绘制直线
  * @param  Graphic 图形数据
						Graphic_Name 图形名
						Operate_Type	操作类型
						Graphic_Layer 图层
						Graphic_Color 图形颜色
						Graphic_Width	线宽
						Start_x，Start_y 开始坐标
						End_x，End_y 结束坐标
  * @retval 
  * @notes  
  */
void Draw_Line(ext_student_custom_graphic_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,u32 Graphic_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//图形名
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
  * @brief 	绘制圆
  * @param  Graphic 图形数据
						Graphic_Name 图形名
						Operate_Type	操作类型
						Graphic_Layer 图层
						Graphic_Color 图形颜色
						Graphic_Width	线宽
						Start_x，Start_y 圆心坐标
						Radius 半径
  * @retval 
  * @notes  
  */
void Draw_Circle(ext_student_custom_graphic_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,u32 Graphic_Width,u32 Start_x,u32 Start_y,u32 Radius)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//图形名
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
  * @brief 	绘制直线
  * @param  Graphic 图形数据
						Graphic_Name 图形名
						Operate_Type	操作类型
						Graphic_Layer 图层
						Graphic_Color 图形颜色
						Graphic_Width	线宽
						Start_x，Start_y 开始坐标
						End_x，End_y 对角顶点坐标
  * @retval 
  * @notes  
  */
void Draw_Rect(ext_student_custom_graphic_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,u32 Graphic_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//图形名
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
  * @brief 	绘制字符
  * @param  Graphic 图形数据
						Graphic_Name 图形名
						Operate_Type	操作类型
						Graphic_Layer 图层
						Graphic_Color 图形颜色
						Graphic_Width	线宽
						Start_x，Start_y 开始坐标
						End_x，End_y 对角顶点坐标
						Font_Size 字体大小
						Graphic_Char 字符串
  * @retval 
  * @notes  
  */
void Draw_Char(ext_client_custom_char_int_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,char* Graphic_Char,u32 Font_Size,u32 Graphic_Width,u32 Start_x,u32 Start_y)
{
	int i;
	Graphic->data_cmd_id = 0x0110;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//图形名
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
  * @brief 	绘制整数
  * @param  Graphic 图形数据
						Graphic_Name 图形名
						Operate_Type	操作类型
						Graphic_Layer 图层
						Graphic_Color 图形颜色
						Graphic_Width	线宽
						Start_x，Start_y 开始坐标
						int_number 32位整型数
  * @retval 
  * @notes  
  */
void Draw_Int(ext_client_custom_graphic_int_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,int int_number,u32 Font_Size,u32 Graphic_Width,u32 Start_x,u32 Start_y)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//图形名
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
  * @brief 	绘制浮点数
  * @param  Graphic 图形数据
						Graphic_Name 图形名
						Operate_Type	操作类型
						Graphic_Layer 图层
						Graphic_Color 图形颜色
						Graphic_Width	线宽
						Start_x，Start_y 开始坐标
						float_number	32位浮点数
						useful_digits 小数位有效个数
  * @retval 
  * @notes  
  */
void Draw_Float(ext_client_custom_graphic_float_t *Graphic,char Graphic_Name[3],u32 Operate_Type,u32 Graphic_Layer,u32 Graphic_Color,float float_number,int useful_digits,u32 Font_Size,u32 Graphic_Width,u32 Start_x,u32 Start_y)
{
	int i;
	Graphic->data_cmd_id = 0x0101;
	Graphic->sender_ID = Sendor_ID;
	Graphic->receiver_ID = Receiver_ID;
	//图形名
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



////静态函数，仅此文件可用，将裁判系统包发送函数在此重定向
//static void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len)
//{
//	Referee_Data_Send(cmd_id,(uint8_t *)&p_data);
//}


char Send_Once_Flag = 0;//初始化标志
extern int Pitch_Angle_NUC;
extern Chassis_Mode_Enum Chassis_Mode;
extern Gimbal_Mode_Enum Gimbal_Mode;
extern Shoot_Mode_Enum Shoot_Mode;
//显示UI界面图形
void Show_UI()
{
	//机器人ID设置
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
	
	//内存清除
	memset(char_one,'\0',sizeof(char_one));
	memset(char_two,'\0',sizeof(char_two));
	memset(char_three,'\0',sizeof(char_three));
	memset(char_four,'\0',sizeof(char_four));
	memset(char_five,'\0',sizeof(char_five));
	memset(char_six,'\0',sizeof(char_six));


	//只需要发送一次的数据，即添加图形操作
	if(Send_Once_Flag == 0)
	{
		
	Send_Once_Flag = 1;//标志位置1,不再进行添加操作
		
	//内存数据置空
	//将数据写入字符串变量（同printf操作）
	sprintf(char_one,"Super Cap %.2f",100.0);
	//将字符图形数据初始化
	Draw_Char(&character_one,"001",Graphic_Operate_ADD,9,Graphic_Color_Yellow,char_one,12,1,80,780);
	
	//同上
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
	
		//初始化整数数据
	Draw_Int(&interger_one,"301",Graphic_Operate_ADD,9,Graphic_Color_Yellow,power_heat_data.shooter_id1_42mm_cooling_heat,12,1,80,820);
	
	//初始化浮点数数据
	Draw_Float(&float_one,"401",Graphic_Operate_ADD,9,Graphic_Color_Yellow,real_shoot_data.bullet_speed,3,12,1,80,840);
	
	//初始化直线数据
	Draw_Line(&line_one,"002",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2,SCREEN_WIDTH/2,SCREEN_LENGTH/2,SCREEN_WIDTH/2-500);
	Draw_Line(&line_two,"003",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-100,SCREEN_WIDTH/2,SCREEN_LENGTH/2+100,SCREEN_WIDTH/2);
	Draw_Line(&line_three,"004",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-80,SCREEN_WIDTH/2-80,SCREEN_LENGTH/2+80,SCREEN_WIDTH/2-80);
	Draw_Line(&line_four,"005",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-60,SCREEN_WIDTH/2-160,SCREEN_LENGTH/2+60,SCREEN_WIDTH/2-160);
	Draw_Line(&line_five,"006",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-40,SCREEN_WIDTH/2-220,SCREEN_LENGTH/2+40,SCREEN_WIDTH/2-220);

	Draw_Line(&line_six,"007",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-40,SCREEN_WIDTH/2,SCREEN_LENGTH/2-40,SCREEN_WIDTH/2-210);
	Draw_Line(&line_seven,"008",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2+40,SCREEN_WIDTH/2,SCREEN_LENGTH/2+40,SCREEN_WIDTH/2-210);

	Draw_Line(&line_eight,"009",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-90,SCREEN_WIDTH/2-40,SCREEN_LENGTH/2+90,SCREEN_WIDTH/2-40);
	Draw_Line(&line_nine,"010",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2-70,SCREEN_WIDTH/2-120,SCREEN_LENGTH/2+70,SCREEN_WIDTH/2-120);

	//初始化圆形数据
	Draw_Circle(&circle_one,"200",Graphic_Operate_ADD,9,Graphic_Color_Yellow,1,SCREEN_LENGTH/2+60,SCREEN_WIDTH/2-160,20);

	//向裁判系统串口发送图形数据
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
	//需要不断更新的数据发送
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
	
	if(Shoot_Mode == SHOOT_MODE_RUN)//摩擦轮开启模式下
	{
	sprintf(char_five,"Fric status on   ");
	if(abs(CAN_Shoot[0].Current_Speed) < 10)//摩擦轮堵住了
	{
	sprintf(char_five,"Fric status block");	
	}
		Draw_Char(&character_five,"103",Graphic_Operate_CHANGE,9,Graphic_Color_Yellow,char_five,12,1,80,860);				
	}
	else if(Shoot_Mode == SHOOT_MODE_STOP)//摩擦轮关闭
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