#ifndef __MAIN_H
#define __MAIN_H

/*********************************************
Copyright(C) ˮľ��Ming
All rights reserved
File name:	main.h
Author:		shuimujieming
Version:		1.0
Description:	�������ļ�����ͷ�ļ�
Others:		
Log:	
*********************************************/

#include "sys.h" //����λ������
#include "stdio.h"	//��׼���������
#include <math.h>   //��ѧ������

#include "arm_math.h" //DSP��ѧ�����


/* �����������͵����ͱ������� */
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;



//FreeRTOS����ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
//����ϵͳ����ͷ�ļ�
#include "task_init.h"
#include "led_task.h"
#include "imu_task.h"
#include "shoot_task.h"
#include "gimbal_task.h"
#include "chassis_task.h"
#include "ui_task.h"
#include "can_send_task.h"
#include "mode_task.h"
#include "supercap_task.h"
#include "nuc_task.h"



//����������
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "laser.h"
#include "dr16.h"
#include "tim6.h"
#include "pid.h"
#include "can.h"
#include "spi.h"
#include "imu.h"
#include "nuc.h"
#include "shoot.h"
#include "cover.h"
#include "referee.h"
#include "beep.h"
#include "bmi088.h"
#include "bmi088driver.h"
#include "flash.h"
#include "rudder.h"

//����ϵͳ
#include "referee_info.h"
#include "crc.h"
#include "custom_ui.h"

//��̨���̿���
#include "gimbal.h"
#include "chassis.h"
#include "can_handle.h"

//��������
#include "ramp.h"
#include "kalman.h"

#endif