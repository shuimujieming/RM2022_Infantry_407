#ifndef __REFEREE_INFO_H
#define __REFEREE_INFO_H

#include "main.h"
void RefereeInfo_Decode(uint8_t *Referee_Info);


#define MAX_SIZE          200    //上传数据寄存器最大的长度
#define frameheader_len  5       //帧头长度
#define cmd_len          2       //命令码长度
#define crc_len          2       //CRC16校验

//帧头格式
typedef struct
{                                       //字节偏移     大小     说明
	u8 SOF;                               //   0          1      数据帧起始字节，固定值为0XA5
	u16 DataLength;                       //   1          2      数据帧内Data长度 
	u8 Seq;                               //   3          1      包序号 
	u8 CRC8;                              //   4          1      帧头CRC校验
}FrameHeader_TypeDef;

//通信协议格式
typedef struct
{                                       //字节偏移     大小     说明
	FrameHeader_TypeDef FrameHeader;      //   0          5      帧头协议
	u16 CmdID;                            //   5          2      命令码ID
	u16 Date[50];                         //   7          n      数据
	u16 FrameTail;                        //   7+n        2      CRC16校验
}RefereeInfo_TypeDef;




#define GAME_STATE_ID         = 0x0001	//比赛状态数据
#define GAME_RESULT_ID        = 0x0002	//比赛结果数据
#define ROBO_SURVIVOR_ID      = 0x0003	//比赛机器人血量数据
#define DART_STATUS_ID      = 0x0004	//飞镖发射状态

#define EVENE_DATA_ID	        = 0x0101	//场地事件数据
#define SUPPLY_ACTION_ID      = 0x0102	//场地补给站动作标识数据
#define SUPPLY_BOOKING_ID     = 0x0103	//请求补给站补弹数据
#define REFEREE_WARNING_ID     = 0x0104	//裁判警告数据
#define DART_REMAINING_TIME_ID     = 0x0105	//飞镖发射口倒计时

#define GAME_ROBO_STATE_ID    = 0x0201	//机器人状态数据
#define REAL_POWER_HEAT_ID    = 0x0202 	//实时功率热量数据
#define ROBOT_POSITION_ID     = 0x0203	//机器人位置数据
#define BUFF_MUSK_ID          = 0x0204	//机器人增益数据
#define AERIAL_ROBO_ENERGY_ID = 0x0205	//空中机器人能量状态数据
#define ROBOT_HURT_ID         = 0x0206	//伤害状态数据
#define REAL_SHOOT_DATA_ID    = 0x0207	//实时射击数据
#define BULLET_REMAINING_ID    = 0x0208	//子弹剩余发送数，空中机器人以及哨兵机器人发送
#define DFID_STATUS_ID    = 0x0209			//机器人 RFID 状态

#define INTERACTIVE_DATA_ID   = 0x0301
	



//比赛状态数据：0X0001  1HZ
typedef __packed struct          
{
  uint8_t game_type : 4;	//比赛类型
	uint8_t game_progress : 4;	//当前比赛阶段
	uint16_t stage_remain_time;	//当前阶段剩余时间
	uint64_t SyncTimeStamp;	//机器人接收到该指令的精确 Unix 时间，当机载端收到有效的 NTP 服务器授时后生效
	
} ext_game_status_t;

//比赛结果数据：0x0002  比赛结束后发送
typedef __packed struct
{
uint8_t winner;	//0 平局 1 红方胜利 2 蓝方胜利

} ext_game_result_t;

//机器人血量数据:0x0003  1Hz
typedef __packed struct
{
uint16_t red_1_robot_HP;
uint16_t red_2_robot_HP;
uint16_t red_3_robot_HP;
uint16_t red_4_robot_HP;
uint16_t red_5_robot_HP;
uint16_t red_7_robot_HP;
uint16_t red_outpost_HP;	//红方前哨战血量
uint16_t red_base_HP;			//红方基地血量
uint16_t blue_1_robot_HP;
uint16_t blue_2_robot_HP;
uint16_t blue_3_robot_HP;
uint16_t blue_4_robot_HP;
uint16_t blue_5_robot_HP;
uint16_t blue_7_robot_HP;
uint16_t blue_outpost_HP;	//蓝方前哨站血量
uint16_t blue_base_HP;	//蓝方基地血量
} ext_game_robot_HP_t;

//飞镖发射状态：0x0004。发送频率：飞镖发射后发送，发送范围：所有机器人
typedef __packed struct
{
uint8_t dart_belong;	//发射飞镖的队伍：1：红方飞镖 2：蓝方飞镖
uint16_t stage_remaining_time;	//发射时的剩余比赛时间，单位s
} ext_dart_status_t;

//场地事件数据： 0x0101  事件改变后发送
typedef __packed struct
{
uint32_t event_type;
} ext_event_data_t;

//补给站动作标识：0x0102   动作改变后发送
typedef __packed struct
{
uint8_t supply_projectile_id;
uint8_t supply_robot_id;
uint8_t supply_projectile_step;
uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

//裁判系统警告信息：0x0104 警告后发送
typedef __packed struct
{
uint8_t level;	//警告等级
uint8_t foul_robot_id;	//犯规机器人 ID
} ext_referee_warning_t;

//飞镖发射口倒计时：cmd_id (0x0105)。发送频率：1Hz 周期发送，发送范围：己方机器人
typedef __packed struct
{
uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;

//比赛机器人状态：0x0201  10Hz
typedef __packed struct
{
uint8_t robot_id;	//机器人ID
uint8_t robot_level;	//机器人等级
uint16_t remain_HP;	//机器人剩余血量
uint16_t max_HP;	//机器人上限血量
uint16_t shooter_id1_17mm_cooling_rate;	//机器人 1 号 17mm 枪口每秒冷却值
uint16_t shooter_id1_17mm_cooling_limit;	//机器人 1 号 17mm 枪口热量上限
uint16_t shooter_id1_17mm_speed_limit;	//机器人 1 号 17mm 枪口上限速度 单位 m/s
uint16_t shooter_id2_17mm_cooling_rate;	//机器人 2 号 17mm 枪口每秒冷却值
uint16_t shooter_id2_17mm_cooling_limit;	//机器人 2 号 17mm 枪口热量上限
uint16_t shooter_id2_17mm_speed_limit;	//机器人 2 号 17mm 枪口上限速度 单位 m/s
uint16_t shooter_id1_42mm_cooling_rate;	//机器人 42mm 枪口每秒冷却值
uint16_t shooter_id1_42mm_cooling_limit;	//机器人 42mm 枪口热量上限
uint16_t shooter_id1_42mm_speed_limit;	//机器人 42mm 枪口上限速度 单位 m/s
uint16_t chassis_power_limit;	//机器人底盘功率限制上限
uint8_t mains_power_gimbal_output : 1;	//主控电源输出情况：1为24v，0为无输出
uint8_t mains_power_chassis_output : 1;
uint8_t mains_power_shooter_output : 1;
} ext_game_robot_status_t;
//实时功率热量数据：0x0202。发送频率：50Hz
typedef __packed struct
{
uint16_t chassis_volt;	//底盘输出电压 单位 毫伏
uint16_t chassis_current;	//底盘输出电流 单位 毫安
float chassis_power;	//底盘输出功率 单位 W 瓦
uint16_t chassis_power_buffer;	//底盘功率缓冲 单位 J 焦耳 备注：飞坡根据规则增加至 250J
uint16_t shooter_id1_17mm_cooling_heat;	//1 号 17mm 枪口热量
uint16_t shooter_id2_17mm_cooling_heat;	//2 号 17mm 枪口热量
uint16_t shooter_id1_42mm_cooling_heat;	//42mm 枪口热量
} ext_power_heat_data_t;
// 机器人位置：0x0203 10Hz
typedef __packed struct
{
float x;
float y;
float z;
float yaw;
} ext_game_robot_pos_t;

//机器人增益：0x0204。发送频率：1Hz
/*
bit 0：机器人血量补血状态
bit 1：枪口热量冷却加速
bit 2：机器人防御加成
bit 3：机器人攻击加成
其他 bit 保留
*/
typedef __packed struct
{
uint8_t power_rune_buff;
}ext_buff_t;

//空中机器人能量状态：0x0205  10Hz
typedef __packed struct
{
uint8_t attack_time;
} aerial_robot_energy_t;

//伤害状态：0x0206    伤害发生后发送
typedef __packed struct
{
uint8_t armor_id : 4;
uint8_t hurt_type : 4;
} ext_robot_hurt_t;

//射击信息：0x0207  射击后发送
typedef __packed struct
{
uint8_t bullet_type;	//子弹类型: 1：17mm 弹丸 2：42mm 弹丸
uint8_t shooter_id;	//发射机构 ID：1：1 号 17mm 发射机构2：2 号 17mm 发射机构 3：42mm 发射机构
uint8_t bullet_freq;	//子弹射频 单位 Hz
float bullet_speed;	//子弹射速 单位 m/s
} ext_shoot_data_t;

//子弹剩余发射数目：0x0208 1HZ
typedef __packed struct
{
uint16_t bullet_remaining_num_17mm;
uint16_t bullet_remaining_num_42mm;
uint16_t coin_remaining_num;
} ext_bullet_remaining_t;

//机器人 RFID 状态：0x0209。发送频率：1Hz，发送范围：单一机器人
typedef __packed struct
{
uint32_t rfid_status;
} ext_rfid_status_t;
// 飞镖机器人客户端指令数据：0x020A。发送频率：10Hz，发送范围：单一机器人
typedef __packed struct
{
uint8_t dart_launch_opening_status;
uint8_t dart_attack_target;
uint16_t target_change_time;
uint8_t first_dart_speed;
uint8_t second_dart_speed;
uint8_t third_dart_speed;
uint8_t fourth_dart_speed;
uint16_t last_dart_launch_time;
uint16_t operate_launch_cmd_time;
} ext_dart_client_cmd_t;

extern ext_game_robot_status_t             game_robot_state;             //0x0201
extern ext_power_heat_data_t 	        		power_heat_data;	        		//0x0202
extern ext_buff_t                          buff_data;                    //0x0204
extern ext_shoot_data_t                    real_shoot_data;              //0x0207
extern ext_bullet_remaining_t              bullet_remaining;             //0x0208

#endif