#ifndef __GIMBAL_TASK_H
#define __GIMBAL_TASK_H
typedef enum
{
	GIMBAL_MODE_STOP = 0,
	GIMBAL_MODE_ROTATE,
	GIMBAL_MODE_FOLLOW,
	GIMBAL_MODE_AUTOAIM,
	GIMBAL_MODE_RUNE
}Gimbal_Mode_Enum;

#include "main.h"
void gimbal_task_create();


#endif