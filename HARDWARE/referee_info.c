#include "referee_info.h"

ext_game_robot_status_t             game_robot_state;             //0x0201
ext_power_heat_data_t 	        		power_heat_data;	        		//0x0202
ext_buff_t                          buff_data;                    //0x0204
ext_shoot_data_t                    real_shoot_data;              //0x0207
ext_bullet_remaining_t              bullet_remaining;             //0x0208

/*
*@title：float8位转32位
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
float bit8TObit32(uint8_t *change_info)
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
/*
*@title：int32位转8位
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
 char bit32TObit8(int index_need,int bit32)
{
	union
	{
    float  f;
		char  byte[4];
	}u32val;
    u32val.f = bit32;
	return u32val.byte[index_need];
}
/*
*@title：8位转16位
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
int16_t bit8TObit16(uint8_t *change_info)
{
	union
	{
    int16_t f;
		char  byte[2];
	}u16val;
    u16val.byte[0]=change_info[0];
    u16val.byte[1]=change_info[1];
	return u16val.f;
}
/*
*@title：int16位转8位
*@description：
*@param 1：	
*@param 2：	
*@return:：	
*/
 char bit16TObit8(int index_need,int bit16)
{
	union
	{
    float  f;
		char  byte[2];
	}u16val;
    u16val.f = bit16;
	return u16val.byte[index_need];
}

/**
 *@Function:		RefereeInfo_Decode(uint8_t *Referee_Info)
 *@Description:	裁判系统数据解析
 *@Param:       形参
 *@Return:	  	返回值
 */
void RefereeInfo_Decode(uint8_t *Referee_Info)
{
	//CMD命令码
	int cmd = bit8TObit16(&Referee_Info[5]);
	//根据命令码接收数据
	switch(cmd)
	{
		case 0x0201:
		{
			memcpy(&game_robot_state,&Referee_Info[7],Referee_Info[2]<<8 | Referee_Info[1]);	
		}break;
		case 0x0202:
		{
			memcpy(&power_heat_data,&Referee_Info[7],Referee_Info[2]<<8 | Referee_Info[1]);
		}break;
		case 0x0204:
		{
			memcpy(&buff_data,&Referee_Info[7],Referee_Info[2]<<8 | Referee_Info[1]);
		}break;
		case 0x0207:
		{
			memcpy(&real_shoot_data,&Referee_Info[7],Referee_Info[2]<<8 | Referee_Info[1]);
		}break;
		case 0x0208:
		{
			memcpy(&bullet_remaining,&Referee_Info[7],Referee_Info[2]<<8 | Referee_Info[1]);
		}break;
		default:
		{
		}
		break;
	}

}

