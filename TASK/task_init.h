#ifndef __TASK_INIT_H
#define __TASK_INIT_H

#include "main.h"
extern EventGroupHandle_t EventGroupHandler;

//�¼����־λ
#define GIMBAL_SIGNAL   ( 1 << 0 )
#define CHASSIS_SIGNAL ( 1 << 1 )
#define SHOOT_SIGNAL     ( 1 << 2 )
#define SUPERCAP_SIGNAL     ( 1 << 3 )
#define SHOOT_EXE_SIGNAL    (1 << 4)

#define UI_SIGNAL (1<<6)
//��ʼ����
#define START_TASK_PRIO		1
#define START_STK_SIZE		256

//LED����
#define LED_TASK_PRIO		2
#define LED_STK_SIZE		128

//��������
#define CHASSIS_TASK_PRIO		3
#define CHASSIS_STK_SIZE		1024

//��������
#define SHOOT_TASK_PRIO		4
#define SHOOT_STK_SIZE		1024

//��̨����
#define GIMBAL_TASK_PRIO		5
#define GIMBAL_STK_SIZE		1024

//����������
#define IMU_TASK_PRIO		6
#define IMU_STK_SIZE		1024

//CAN���ݷ�������
#define CAN_SEND_TASK_PRIO		7
#define CAN_SEND_STK_SIZE		1024

//ģʽ״̬��������
#define MODE_TASK_PRIO		7
#define MODE_STK_SIZE		1024

//������������
#define SUPERCAP_TASK_PRIO		8
#define SUPERCAP_STK_SIZE		1024

//�Զ���UI����
#define UI_TASK_PRIO		2
#define UI_STK_SIZE		1024

//NUC����
#define NUC_TASK_PRIO		9
#define NUC_STK_SIZE		1024
void Task_Init();
#endif