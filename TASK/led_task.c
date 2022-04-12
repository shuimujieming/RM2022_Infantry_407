#include "led_task.h"

TaskHandle_t LED_Task_Handler;
void led_task(void *p_arg);

/**
 *@Function:		led_task_create()
 *@Description:	LED任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void led_task_create()
{
		xTaskCreate((TaskFunction_t )led_task,          //任务函数
							(const char*    )"led_task",          //任务名称
							(uint16_t       )LED_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )LED_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&LED_Task_Handler);  //任务句柄  
}

/**
 *@Function:		led_task(void *p_arg)
 *@Description:	LED任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void led_task(void *p_arg)
{
	int led_frequency = 0;
	
	while(1)
	{
		//led灯频率控制
		led_frequency++;
		
		if(led_frequency % 1 == 0)
		{LED_Run();}
		
		//蜂鸣器响声
		if(led_frequency == 1)
		{
			Beep_Ctrl(1000,10);			
		}
		else if(led_frequency == 2)
		{
			Beep_Ctrl(800,10);						
		}
		else if(led_frequency == 3)
		{
			Beep_Ctrl(600,10);						
		}
		else if(led_frequency == 4)
		{
			Beep_Ctrl(400,10);						
		}
		else
		{
			Beep_Ctrl(10000,0);									
		}	
	
		vTaskDelay(500);
	}
}