#include "imu_task.h"


TaskHandle_t IMU_Task_Handler;
void imu_task(void *p_arg);

/**
 *@Function:		imu_task_create()
 *@Description:	IMU任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void imu_task_create()
{
		xTaskCreate((TaskFunction_t )imu_task,          //任务函数
							(const char*    )"imu_task",          //任务名称
							(uint16_t       )IMU_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )IMU_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&IMU_Task_Handler);  //任务句柄  
}

/**
 *@Function:		imu_task(void *p_arg)
 *@Description:	IMU任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void imu_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(1);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	while(1)
	{
		
		BMI088_Get_Data();//陀螺仪数据更新
		IMU_Ahrs_Update();//姿态解算
		IMU_Attitude_Update();//欧拉角转化
		
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}