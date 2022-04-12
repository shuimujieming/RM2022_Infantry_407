#ifndef __LASER_H
#define __LASER_H
#include "main.h"


//打开激光
#define LASER_ON 	GPIO_SetBits(GPIOG,GPIO_Pin_13)
//关闭激光
#define LASER_OFF 	GPIO_ResetBits(GPIOG,GPIO_Pin_13)

void Laser_Init(void);

#endif