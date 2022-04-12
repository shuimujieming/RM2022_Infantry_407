#ifndef __CRC_H
#define __CRC_H
#include "main.h"

#define CRC_Check_False   0
#define CRC_Check_True    1

uint8_t Get_CRC8_Check_Sum(uint8_t* pchMessage, uint16_t dwLength, uint8_t ucCRC8);
uint8_t Verify_CRC8_Check_Sum(uint8_t* pchMessage, uint16_t dwLength);
void Append_CRC8_Check_Sum(uint8_t* pchMessage, uint16_t dwLength);

uint16_t Get_CRC16_Check_Sum(uint8_t* pchMessage, uint32_t dwLength, uint16_t wCRC);
uint8_t Verify_CRC16_Check_Sum(uint8_t* pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t* pchMessage, uint32_t dwLength);

#endif

