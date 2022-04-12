#include "shoot.h"

/**
 *@Function:		Shoot_Init()
 *@Description:	发射参数初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void Shoot_Init()
{
	Pid_Reset(&PID_Shoot[0]);
	Pid_Reset(&PID_Shoot[1]);
	Pid_Reset(&PID_Trigger);
	
	
	Pid_Set(&PID_Shoot[0],10,0,0,16000,50,16000,16000,1,16000,0,2); 
	Pid_Set(&PID_Shoot[1],10,0,0,16000,50,16000,16000,1,16000,0,2);
	Pid_Set(&PID_Trigger,10,0,0,10000,50,10000,10000,1,10000,0,2);

	Pid_Set(&PID_Trigger_Angle,0.2,0,0,16000,50,16000,16000,1,16000,0,2); 
	Pid_Set(&PID_Trigger_Speed,2,0,0,16000,50,16000,16000,1,16000,0,2);
}