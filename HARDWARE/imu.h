#ifndef __IMU_H
#define __IMU_H
#include "sys.h"
#include <math.h>//��ѧ����
#include "mpu6500.h"//�������������
#include "ist8310.h"//�����ƼĴ�����ַ���ñ�
#include "delay.h"//��ʱ
#include "tim6.h"

void IMU_Init(void);
void IMU_DATA_UPDATE(void);
void IMU_Get_Data(void);
void IMU_Ahrs_Update();
void IMU_Attitude_Update();

#endif

