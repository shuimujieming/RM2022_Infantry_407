#ifndef __IMU_H
#define __IMU_H
#include "sys.h"
#include <math.h>//数学函数
#include "mpu6500.h"//陀螺仪相关配置
#include "ist8310.h"//磁力计寄存器地址配置表
#include "delay.h"//延时
#include "tim6.h"

void IMU_Init(void);
void IMU_DATA_UPDATE(void);
void IMU_Get_Data(void);
void IMU_Ahrs_Update();
void IMU_Attitude_Update();

#endif

