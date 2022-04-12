#ifndef __CHASSIS_TASK_H
#define __CHASSIS_TASK_H

#define MAX_WHEEL_SPEED 8500


typedef struct
{
	float vx;
	float vy;
	float vw;
	short wheel_speed[4];
}Chassis_Speed_Typedef;
typedef struct
{
	float vx;
	float vy;
	float vw;
} Chassis_Control_Speed_Typedef;
typedef enum
{
	CHASSIS_MODE_STOP = 0,
	CHASSIS_MODE_ROTATE,
	CHASSIS_MODE_FOLLOW,
	CHASSIS_MODE_AUTOAIM,
	CHASSIS_MODE_SUPERCAP,
	CHASSIS_MODE_RUNE
}Chassis_Mode_Enum;

#include "main.h"
void chassis_task_create();


#endif