#include "nuc_task.h"

TaskHandle_t NUC_Task_Handler;
void nuc_task(void *p_arg);

/**
 *@Function:		nuc_task_create()
 *@Description:	NUC���񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void nuc_task_create()
{
		xTaskCreate((TaskFunction_t )nuc_task,          //������
							(const char*    )"nuc_task",          //��������
							(uint16_t       )NUC_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )NUC_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&NUC_Task_Handler);  //������  
}

/**
 *@Function:		nuc_task(void *p_arg)
 *@Description:	NUC����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void nuc_task(void *p_arg)
{
	const TickType_t TimeIncrement=pdMS_TO_TICKS(10);
	TickType_t	PreviousWakeTime;
	PreviousWakeTime=xTaskGetTickCount();	
	
	while(1)
	{
		NUC_Send_Data();
		vTaskDelayUntil(&PreviousWakeTime,TimeIncrement);	
	}

}