#ifndef __TIM6_H
#define __TIM6_H
#include "main.h"

void TIM6_Init(int arr);

extern uint32_t imu_tick_counts;//时钟计数值
extern float Yaw_Angle;//随动，积分角度

#endif