#ifndef __RUDDER_H
#define __RUDDER_H


typedef struct 
{
	float XY_Angle_Origin;
	float Yaw_Angle_Offset;
	float XY_Angle_Real;
	float XY_Speed;
	float XYZ_Angle[4];
	float XYZ_Angle_Current[4];
	float XYZ_Angle_Target[4];
	float XYZ_Angle_Last[4];
	
	float XYZ_Angle_A[4];
	float XYZ_Angle_B[4];
	float XYZ_Speed[4];
	int   XYZ_Speed_Dir[4];
} Rudder_TypeDef;

extern Rudder_TypeDef Rudder_Data;


#include "main.h"
void Rudder_Init();

#endif