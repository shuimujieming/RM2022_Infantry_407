#include "pid.h"

//底盘PID
PID_TypeDef PID_Chassis[4];

//舵PID
PID_TypeDef PID_Rudder_Angle[4];
PID_TypeDef PID_Rudder_Speed[4];

//云台PID
PID_TypeDef PID_Gimbal_Angle[2];
PID_TypeDef PID_Gimbal_Speed[2];
PID_TypeDef PID_Chassis_Omega;

//发射PID
PID_TypeDef PID_Shoot[2];
//拨弹盘连发速度环
PID_TypeDef PID_Trigger;
//拨弹盘单发角度环
PID_TypeDef PID_Trigger_Angle;
PID_TypeDef PID_Trigger_Speed;

/**
 *@Function:		Pid_Set
(  
	 PID_TypeDef *PID,float Kp,float Ki,float Kd,
	 float Pmax,float Imax,float Dmax,float PIDmax,
	 float Speed_Ratio,float Acceletation,
	 float Error_Ignored,u8 Motor_Type)
 *@Description:	PID初始化赋值
 *@Param:       形参
 *@Return:	  	返回值
 */
void Pid_Set
(  
	 PID_TypeDef *PID,float Kp,float Ki,float Kd,
	 float Pmax,float Imax,float Dmax,float PIDmax,
	 float Speed_Ratio,float Acceletation,
	 float Error_Ignored,u8 Motor_Type)
{
		PID->P=Kp;
		PID->I=Ki;
		PID->D=Kd;

		PID->PMax=Pmax;
		PID->IMax=Imax;
		PID->DMax=Dmax;
		PID->PIDOutMax=PIDmax;
	
		PID->ErrorIgnored=Error_Ignored;

		PID->Speed_Ratio=Speed_Ratio;
		PID->Acceleration=Acceletation;
	
		PID->Motor_Type = Motor_Type;
}

/**
 *@Function:		Pid_Reset(PID_TypeDef *PID)
 *@Description:	PID重置
 *@Param:       形参
 *@Return:	  	返回值
 */
void Pid_Reset(PID_TypeDef *PID)
{
		PID->P=0;
		PID->I=0;
		PID->D=0;
		PID->CurrentError=0;
		PID->LastError=0;
		PID->ErrorIgnored=0;
		PID->Pout=0;
		PID->Iout=0;
		PID->Dout=0;
		PID->PIDout=0;
		PID->PIDOutCompensate=0;
		PID->PMax=0;
		PID->IMax=0;
		PID->DMax=0;
		PID->PIDOutMax=0;
		PID->PIDOutLast=0;
		PID->Target_Speed_Last=0;
		PID->Speed_Ratio=1;
		PID->Acceleration=0;
}
/**
 *@Function:		Pid_Calc(PID_TypeDef *PID, float Current_Speed, float Target_Speed)
 *@Description:	PID计算
 *@Param:       形参
 *@Return:	  	返回值
 */
float Pid_Calc(PID_TypeDef *PID, float Current_Speed, float Target_Speed)
{   	
	//速度比例转换
	Current_Speed = Current_Speed / PID->Speed_Ratio;

	//记录上一时刻速度
	PID->Target_Speed_Last = Target_Speed;
	
	//误差值计算
	PID->CurrentError = Target_Speed - Current_Speed;
	
	//死区忽略	
	if (fabs(PID->CurrentError) < PID->ErrorIgnored )
	{PID->CurrentError = 0;}
	else if(PID->CurrentError > 0)
	{PID->CurrentError -= PID->ErrorIgnored;}
	else if(PID->CurrentError < 0)
	{PID->CurrentError += PID->ErrorIgnored;}
	
	//Pout  
	PID->Pout = PID->P * PID->CurrentError;
	if(PID->Pout >  PID->PMax)  PID->Pout =  PID->PMax;
	if(PID->Pout < -PID->PMax)  PID->Pout = -PID->PMax;
	
	//Iout
	PID->Iout += PID->I * PID->CurrentError;
	if(PID->Iout >  PID->IMax) PID->Iout =  PID->IMax;
	if(PID->Iout < -PID->IMax) PID->Iout = -PID->IMax;
	
	//Dout 
	PID->Dout = PID->D * ( PID->CurrentError - PID->LastError );
	if(PID->Dout >  PID->DMax) PID->Dout =  PID->DMax;
	if(PID->Dout < -PID->DMax) PID->Dout = -PID->DMax;
	
	//PIDout
	PID->PIDout = PID->Pout + PID->Iout + PID->Dout + PID->PIDOutCompensate;

	//输出限幅
	if(PID->PIDout >  PID->PIDOutMax) PID->PIDout =  PID->PIDOutMax;
	if(PID->PIDout < -PID->PIDOutMax) PID->PIDout = -PID->PIDOutMax;

	//上一时刻数据保存
  PID->PIDOutLast = PID->PIDout;
	PID->LastError  = PID->CurrentError;
	
	return PID->PIDout;    
}
