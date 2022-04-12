#include "led_task.h"

TaskHandle_t LED_Task_Handler;
void led_task(void *p_arg);

/**
 *@Function:		led_task_create()
 *@Description:	LED���񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void led_task_create()
{
		xTaskCreate((TaskFunction_t )led_task,          //������
							(const char*    )"led_task",          //��������
							(uint16_t       )LED_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )LED_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&LED_Task_Handler);  //������  
}

/**
 *@Function:		led_task(void *p_arg)
 *@Description:	LED����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void led_task(void *p_arg)
{
	int led_frequency = 0;
	
	while(1)
	{
		//led��Ƶ�ʿ���
		led_frequency++;
		
		if(led_frequency % 1 == 0)
		{LED_Run();}
		
		//����������
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