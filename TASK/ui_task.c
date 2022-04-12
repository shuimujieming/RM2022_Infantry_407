#include "ui_task.h"

TaskHandle_t UI_Task_Handler;
void ui_task(void *p_arg);

/**
 *@Function:		ui_task_create()
 *@Description:	UI任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void ui_task_create()
{
		xTaskCreate((TaskFunction_t )ui_task,          //任务函数
							(const char*    )"ui_task",          //任务名称
							(uint16_t       )UI_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )UI_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&UI_Task_Handler);  //任务句柄  
}
/**
 *@Function:		ui_task(void *p_arg)
 *@Description:	UI任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void ui_task(void *p_arg)
{	

	while(1)
	{
		//绘制UI
		Show_UI();
		//带宽30Hz
		vTaskDelay(20);
	}
}