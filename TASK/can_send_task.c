#include "can_send_task.h"


TaskHandle_t CAN_SEND_Task_Handler;
void can_send_task(void *p_arg);

/**
 *@Function:		can_send_task_create()
 *@Description:	CAN数据发送任务创建
 *@Param:       形参
 *@Return:	  	返回值
 */
void can_send_task_create()
{
		xTaskCreate((TaskFunction_t )can_send_task,          //任务函数
							(const char*    )"can_send_task",          //任务名称
							(uint16_t       )CAN_SEND_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                //传递给任务函数的参数
							(UBaseType_t    )CAN_SEND_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&CAN_SEND_Task_Handler);  //任务句柄  
}

/**
 *@Function:		can_send_task(void *p_arg)
 *@Description:	CAN数据发送任务
 *@Param:       形参
 *@Return:	  	返回值
 */
void can_send_task(void *p_arg)
{

	EventBits_t  EventValue;

	
	while(1)
	{
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)EventGroupHandler,
																	(EventBits_t      )CHASSIS_SIGNAL | GIMBAL_SIGNAL | SUPERCAP_SIGNAL,
																	(BaseType_t       )pdTRUE,
																	(BaseType_t       )pdFALSE,
																	(TickType_t       )portMAX_DELAY);
		
		//云台、底盘、弹仓、发射、超级电容CAN数据等待发送
		if(EventValue & GIMBAL_SIGNAL)
		{
			CAN1_TX_Gimbal_Yaw();
			CAN1_TX_Gimbal_Pitch();
			CAN2_TX_Shoot();
		}
		else if(EventValue & CHASSIS_SIGNAL)
		{
			CAN1_TX_Chassis();
			CAN2_TX_Rudder();
		}
		else if(EventValue & SUPERCAP_SIGNAL)
		{
			CAN1_TX_SuperCap();
		}
	}

}