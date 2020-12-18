/*******************************************************************************
                      ��Ȩ���� (C), 2020-,NCUROBOT
 *******************************************************************************
  �� �� ��   : offline_check_task.c
  �� �� ��   : V1.0
  ��    ��   : ���ƺ�
  ��������   : 2020.12.10
  ����޸�   :
  ��������   : ���߼���������ڵ���������߼�⺯����������߼�⺯�����ж�
							 ����������Ƿ���ߣ������߲�������ش���
							 ע����Ҫ��һ������
  �����б�   : 1) OffLine_Check_Task()			��FreeRTOS����������ϵͳ����������С�
*******************************************************************************/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "offline_check_task.h"
#include "offline_check.h"
/* �ڲ��궨�� ----------------------------------------------------------------*/

/* �ڲ��Զ����������͵ı��� --------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/

/* �ڲ�����ԭ������ ----------------------------------------------------------*/

/* �������岿�� --------------------------------------------------------------*/
/**
  * @brief				���߼������
  * @param[out]		
  * @param[in]		
  * @retval				
*/
void OffLine_Check_Task(void const *argument)
{
	osDelay(1000);

	portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{		
		Refresh_Task_OffLine_Time(OutLineCheckTask_TOE);
		
		Task_OffLine_Check();//������
		Device_OffLine_Check();//���߼��
		
		//������߶�Ӧ����
		if((off_line.task_offline_flag & 0x01))//ң�����������
		{
			printf("Remote_Data_Task is offline\r\n");
		}
				
		if((off_line.task_offline_flag & 0x02))//�Ӿ��������
		{
			printf("Vision_Data_Task is offline\r\n");
		} 
		
		if((off_line.task_offline_flag & 0x04))//����ϵͳ����
		{
			printf("Referee_Data_Task is offline\r\n");
		}	
		
		if((off_line.task_offline_flag & 0x08))//Ħ�����������
		{
			printf("Friction_Drive_Task is offline\r\n");
		}
		
		if((off_line.task_offline_flag & 0x10))//�����������
		{
			printf("Trigger_Drive_Task is offline\r\n");
		}	

		if((off_line.task_offline_flag & 0x20))//��̨�������
		{
			printf("Gimbal_Control_Task is offline\r\n");
		}	
		
		if((off_line.task_offline_flag & 0x40))//�����������
		{
			printf("Chassis_Control_Task is offline\r\n");
		}	
		
		if((off_line.task_offline_flag & 0x80))//���߼���������
		{
			printf("OffLine_Check_Task is offline\r\n");
		}
		
		
		//������߶�Ӧ����
		if((off_line.device_offline_flag & 0x01))//�޷�����yaw��������ֵ LED1
		{
			LED1_TOG();
//			printf("Cannot receive yaw_motor return data\r\n");
		}
		else 
			LED1_ON();
		
		if((off_line.device_offline_flag & 0x02))//�޷�����pitch��������ֵ LED2
		{
			LED2_TOG();
//			printf("Cannot receive pitch_motor return data\r\n");
		}
		else 
			LED2_ON();
		
		if((off_line.device_offline_flag & 0x04))//�޷����ղ��̵������ֵ LED3
		{
			LED3_TOG();
//			printf("Cannot receive trigger_motor return data\r\n");
		}
		else 
			LED3_ON();		

		if((off_line.device_offline_flag & 0x08))//�޷����յ��̵������ֵ LED1
		{
			LED4_TOG();
//			printf("Cannot receive chassis_motor return data\r\n");
		}
		else 
			LED4_ON();
		
		if((off_line.device_offline_flag & 0x10))//�޷����յ��̵������ֵ LED1
		{
			LED5_TOG();
//			printf("Cannot receive remote_control return data\r\n");
		}
		else 
			LED5_ON();

		if((off_line.device_offline_flag & 0x20))//�޷����յ��̵������ֵ LED1
		{
			LED6_TOG();
//			printf("Cannot receive referee return data\r\n");
		}
		else 
			LED6_ON();		
		
		if((off_line.device_offline_flag & 0x40))//�޷����յ��̵������ֵ LED1
		{
			LED7_TOG();
//			printf("Cannot receive vision return data\r\n");
		}
		else 
			LED7_ON();	
		
		
		osDelayUntil(&xLastWakeTime,OFFLINE_PERIOD);
		
	}
}
