#ifndef __COVER_H
#define __COVER_H

#include "main.h"
void Cover_Init();

//ȫ����
#define COVER_OPEN_VAL 2100
#define COVER_CLOSE_VAL 1180

////����
//#define COVER_OPEN_VAL 2050
//#define COVER_CLOSE_VAL 1100

#define COVER_ON	TIM1->CCR1 = COVER_OPEN_VAL
#define COVER_OFF	TIM1->CCR1 = COVER_CLOSE_VAL


#endif