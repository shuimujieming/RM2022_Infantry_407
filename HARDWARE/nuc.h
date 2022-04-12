#ifndef __NUC_H
#define __NUC_H



#include "main.h"
void NUC_Init();

typedef  struct
{
	uint8_t header;
	float pit_offset;
	float yaw_offset;
	uint8_t buff_flag;
	uint8_t tail;
}NUC_Info_TypeDef;

void NUC_Send_Data();
extern NUC_Info_TypeDef NUC_Data;

#endif