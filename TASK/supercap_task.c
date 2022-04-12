#include "supercap_task.h"

TaskHandle_t SUPERCAP_Task_Handler;
void supercap_task(void *p_arg);

/**
 *@Function:		supercap_task_create()
 *@Description:	超级电容任务控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void supercap_task_create()
{
		xTaskCreate((TaskFunction_t )supercap_task,          //任务函数
							(const char*    )"supercap_task",          //任务名称
							(uint16_t       )SUPERCAP_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )SUPERCAP_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&SUPERCAP_Task_Handler);  //任务句柄  
}

/**
 *@Function:		supercap_task(void *p_arg)
 *@Description:	超级电容任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void supercap_task(void *p_arg)
{
	//推荐10Hz
	const TickType_t TimeIncrement=pdMS_TO_TICKS(100);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	while(1)
	{
		xEventGroupSetBits(EventGroupHandler,SUPERCAP_SIGNAL);	//超级电容事件组置位
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}