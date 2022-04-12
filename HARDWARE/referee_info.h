#ifndef __REFEREE_INFO_H
#define __REFEREE_INFO_H

#include "main.h"
void RefereeInfo_Decode(uint8_t *Referee_Info);


#define MAX_SIZE          200    //�ϴ����ݼĴ������ĳ���
#define frameheader_len  5       //֡ͷ����
#define cmd_len          2       //�����볤��
#define crc_len          2       //CRC16У��

//֡ͷ��ʽ
typedef struct
{                                       //�ֽ�ƫ��     ��С     ˵��
	u8 SOF;                               //   0          1      ����֡��ʼ�ֽڣ��̶�ֵΪ0XA5
	u16 DataLength;                       //   1          2      ����֡��Data���� 
	u8 Seq;                               //   3          1      ����� 
	u8 CRC8;                              //   4          1      ֡ͷCRCУ��
}FrameHeader_TypeDef;

//ͨ��Э���ʽ
typedef struct
{                                       //�ֽ�ƫ��     ��С     ˵��
	FrameHeader_TypeDef FrameHeader;      //   0          5      ֡ͷЭ��
	u16 CmdID;                            //   5          2      ������ID
	u16 Date[50];                         //   7          n      ����
	u16 FrameTail;                        //   7+n        2      CRC16У��
}RefereeInfo_TypeDef;




#define GAME_STATE_ID         = 0x0001	//����״̬����
#define GAME_RESULT_ID        = 0x0002	//�����������
#define ROBO_SURVIVOR_ID      = 0x0003	//����������Ѫ������
#define DART_STATUS_ID      = 0x0004	//���ڷ���״̬

#define EVENE_DATA_ID	        = 0x0101	//�����¼�����
#define SUPPLY_ACTION_ID      = 0x0102	//���ز���վ������ʶ����
#define SUPPLY_BOOKING_ID     = 0x0103	//���󲹸�վ��������
#define REFEREE_WARNING_ID     = 0x0104	//���о�������
#define DART_REMAINING_TIME_ID     = 0x0105	//���ڷ���ڵ���ʱ

#define GAME_ROBO_STATE_ID    = 0x0201	//������״̬����
#define REAL_POWER_HEAT_ID    = 0x0202 	//ʵʱ������������
#define ROBOT_POSITION_ID     = 0x0203	//������λ������
#define BUFF_MUSK_ID          = 0x0204	//��������������
#define AERIAL_ROBO_ENERGY_ID = 0x0205	//���л���������״̬����
#define ROBOT_HURT_ID         = 0x0206	//�˺�״̬����
#define REAL_SHOOT_DATA_ID    = 0x0207	//ʵʱ�������
#define BULLET_REMAINING_ID    = 0x0208	//�ӵ�ʣ�෢���������л������Լ��ڱ������˷���
#define DFID_STATUS_ID    = 0x0209			//������ RFID ״̬

#define INTERACTIVE_DATA_ID   = 0x0301
	



//����״̬���ݣ�0X0001  1HZ
typedef __packed struct          
{
  uint8_t game_type : 4;	//��������
	uint8_t game_progress : 4;	//��ǰ�����׶�
	uint16_t stage_remain_time;	//��ǰ�׶�ʣ��ʱ��
	uint64_t SyncTimeStamp;	//�����˽��յ���ָ��ľ�ȷ Unix ʱ�䣬�����ض��յ���Ч�� NTP ��������ʱ����Ч
	
} ext_game_status_t;

//����������ݣ�0x0002  ������������
typedef __packed struct
{
uint8_t winner;	//0 ƽ�� 1 �췽ʤ�� 2 ����ʤ��

} ext_game_result_t;

//������Ѫ������:0x0003  1Hz
typedef __packed struct
{
uint16_t red_1_robot_HP;
uint16_t red_2_robot_HP;
uint16_t red_3_robot_HP;
uint16_t red_4_robot_HP;
uint16_t red_5_robot_HP;
uint16_t red_7_robot_HP;
uint16_t red_outpost_HP;	//�췽ǰ��սѪ��
uint16_t red_base_HP;			//�췽����Ѫ��
uint16_t blue_1_robot_HP;
uint16_t blue_2_robot_HP;
uint16_t blue_3_robot_HP;
uint16_t blue_4_robot_HP;
uint16_t blue_5_robot_HP;
uint16_t blue_7_robot_HP;
uint16_t blue_outpost_HP;	//����ǰ��վѪ��
uint16_t blue_base_HP;	//��������Ѫ��
} ext_game_robot_HP_t;

//���ڷ���״̬��0x0004������Ƶ�ʣ����ڷ�����ͣ����ͷ�Χ�����л�����
typedef __packed struct
{
uint8_t dart_belong;	//������ڵĶ��飺1���췽���� 2����������
uint16_t stage_remaining_time;	//����ʱ��ʣ�����ʱ�䣬��λs
} ext_dart_status_t;

//�����¼����ݣ� 0x0101  �¼��ı����
typedef __packed struct
{
uint32_t event_type;
} ext_event_data_t;

//����վ������ʶ��0x0102   �����ı����
typedef __packed struct
{
uint8_t supply_projectile_id;
uint8_t supply_robot_id;
uint8_t supply_projectile_step;
uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

//����ϵͳ������Ϣ��0x0104 �������
typedef __packed struct
{
uint8_t level;	//����ȼ�
uint8_t foul_robot_id;	//��������� ID
} ext_referee_warning_t;

//���ڷ���ڵ���ʱ��cmd_id (0x0105)������Ƶ�ʣ�1Hz ���ڷ��ͣ����ͷ�Χ������������
typedef __packed struct
{
uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;

//����������״̬��0x0201  10Hz
typedef __packed struct
{
uint8_t robot_id;	//������ID
uint8_t robot_level;	//�����˵ȼ�
uint16_t remain_HP;	//������ʣ��Ѫ��
uint16_t max_HP;	//����������Ѫ��
uint16_t shooter_id1_17mm_cooling_rate;	//������ 1 �� 17mm ǹ��ÿ����ȴֵ
uint16_t shooter_id1_17mm_cooling_limit;	//������ 1 �� 17mm ǹ����������
uint16_t shooter_id1_17mm_speed_limit;	//������ 1 �� 17mm ǹ�������ٶ� ��λ m/s
uint16_t shooter_id2_17mm_cooling_rate;	//������ 2 �� 17mm ǹ��ÿ����ȴֵ
uint16_t shooter_id2_17mm_cooling_limit;	//������ 2 �� 17mm ǹ����������
uint16_t shooter_id2_17mm_speed_limit;	//������ 2 �� 17mm ǹ�������ٶ� ��λ m/s
uint16_t shooter_id1_42mm_cooling_rate;	//������ 42mm ǹ��ÿ����ȴֵ
uint16_t shooter_id1_42mm_cooling_limit;	//������ 42mm ǹ����������
uint16_t shooter_id1_42mm_speed_limit;	//������ 42mm ǹ�������ٶ� ��λ m/s
uint16_t chassis_power_limit;	//�����˵��̹�����������
uint8_t mains_power_gimbal_output : 1;	//���ص�Դ��������1Ϊ24v��0Ϊ�����
uint8_t mains_power_chassis_output : 1;
uint8_t mains_power_shooter_output : 1;
} ext_game_robot_status_t;
//ʵʱ�����������ݣ�0x0202������Ƶ�ʣ�50Hz
typedef __packed struct
{
uint16_t chassis_volt;	//���������ѹ ��λ ����
uint16_t chassis_current;	//����������� ��λ ����
float chassis_power;	//����������� ��λ W ��
uint16_t chassis_power_buffer;	//���̹��ʻ��� ��λ J ���� ��ע�����¸��ݹ��������� 250J
uint16_t shooter_id1_17mm_cooling_heat;	//1 �� 17mm ǹ������
uint16_t shooter_id2_17mm_cooling_heat;	//2 �� 17mm ǹ������
uint16_t shooter_id1_42mm_cooling_heat;	//42mm ǹ������
} ext_power_heat_data_t;
// ������λ�ã�0x0203 10Hz
typedef __packed struct
{
float x;
float y;
float z;
float yaw;
} ext_game_robot_pos_t;

//���������棺0x0204������Ƶ�ʣ�1Hz
/*
bit 0��������Ѫ����Ѫ״̬
bit 1��ǹ��������ȴ����
bit 2�������˷����ӳ�
bit 3�������˹����ӳ�
���� bit ����
*/
typedef __packed struct
{
uint8_t power_rune_buff;
}ext_buff_t;

//���л���������״̬��0x0205  10Hz
typedef __packed struct
{
uint8_t attack_time;
} aerial_robot_energy_t;

//�˺�״̬��0x0206    �˺���������
typedef __packed struct
{
uint8_t armor_id : 4;
uint8_t hurt_type : 4;
} ext_robot_hurt_t;

//�����Ϣ��0x0207  �������
typedef __packed struct
{
uint8_t bullet_type;	//�ӵ�����: 1��17mm ���� 2��42mm ����
uint8_t shooter_id;	//������� ID��1��1 �� 17mm �������2��2 �� 17mm ������� 3��42mm �������
uint8_t bullet_freq;	//�ӵ���Ƶ ��λ Hz
float bullet_speed;	//�ӵ����� ��λ m/s
} ext_shoot_data_t;

//�ӵ�ʣ�෢����Ŀ��0x0208 1HZ
typedef __packed struct
{
uint16_t bullet_remaining_num_17mm;
uint16_t bullet_remaining_num_42mm;
uint16_t coin_remaining_num;
} ext_bullet_remaining_t;

//������ RFID ״̬��0x0209������Ƶ�ʣ�1Hz�����ͷ�Χ����һ������
typedef __packed struct
{
uint32_t rfid_status;
} ext_rfid_status_t;
// ���ڻ����˿ͻ���ָ�����ݣ�0x020A������Ƶ�ʣ�10Hz�����ͷ�Χ����һ������
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