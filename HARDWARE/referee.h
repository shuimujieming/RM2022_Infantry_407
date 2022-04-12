#ifndef __REFEREE_H
#define __REFEREE_H

#include "main.h"

void Referee_Init();
void Referee_Data_Send(uint16_t cmd_id, uint8_t* p_data);
void referee_data_pack_handle(uint8_t sof,uint16_t cmd_id, uint8_t *p_data, uint16_t len);
void referee_data_send();

#endif