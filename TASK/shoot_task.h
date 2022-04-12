#ifndef __SHOOT_TASK_H
#define __SHOOT_TASK_H


typedef enum
{
	SHOOT_MODE_STOP = 0,
	SHOOT_MODE_RUN,
}Shoot_Mode_Enum;


//Ϊ1���ǵ�����Ϊ2��������
typedef enum
{
	SHOOT_CMD_STOP = 0,
	SHOOT_CMD_ONCE,
	SHOOT_CMD_LONG,
}Shoot_Cmd_Enum;


#include "main.h"

void shoot_task_create();

#endif