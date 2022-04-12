#include "ui_task.h"

TaskHandle_t UI_Task_Handler;
void ui_task(void *p_arg);

/**
 *@Function:		ui_task_create()
 *@Description:	UI���񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void ui_task_create()
{
		xTaskCreate((TaskFunction_t )ui_task,          //������
							(const char*    )"ui_task",          //��������
							(uint16_t       )UI_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                //���ݸ��������Ĳ���
							(UBaseType_t    )UI_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&UI_Task_Handler);  //������  
}
/**
 *@Function:		ui_task(void *p_arg)
 *@Description:	UI����
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void ui_task(void *p_arg)
{	

	while(1)
	{
		//����UI
		Show_UI();
		//����30Hz
		vTaskDelay(20);
	}
}