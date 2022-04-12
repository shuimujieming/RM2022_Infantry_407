#include "task_init.h"

TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);
EventGroupHandle_t EventGroupHandler;
/**
 *@Function:		start_task_create()
 *@Description:	�������񴴽�
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void start_task_create()
{
		xTaskCreate((TaskFunction_t )start_task,            //������
							(const char*    )"start_task",          //��������
							(uint16_t       )START_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                  //���ݸ��������Ĳ���
							(UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&StartTask_Handler);   //������              
		
	  vTaskStartScheduler();          //�����������			
}
/**
 *@Function:		start_task(void *pvParameters)
 *@Description:	��������
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void start_task(void *pvParameters)
{	
	EventGroupHandler = xEventGroupCreate();	//�����¼���
	
	while(1)
	{
		taskENTER_CRITICAL();									 //�����ٽ���
		
		led_task_create();		                 //����LED������
		
		imu_task_create();	                 	 //����IMU����
		
		gimbal_task_create();                  //������̨����
		
		chassis_task_create();                 //������������
		
		shoot_task_create();                   //������������
		
		nuc_task_create();                     //����NUC����
		
		supercap_task_create();                //����������������
		
		can_send_task_create();                //����CAN���ݷ�������
		
		mode_task_create();		                 //����ģʽ״̬��������
		
		ui_task_create();											 //�����Զ���UI����
		
		vTaskDelete(StartTask_Handler);	       //ɾ����ʼ����
		
		taskEXIT_CRITICAL();	                //�˳��ٽ���
	}
	
}
/**
 *@Function:		Task_Init()
 *@Description:	�����ʼ��
 *@Param:       �β�
 *@Return:	  	����ֵ
 */
void Task_Init()
{
	start_task_create();	//��ʼ����
}