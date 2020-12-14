/*******************************************************************************
                      ��Ȩ���� (C), 2020-,NCUROBOT
 *******************************************************************************
  �� �� ��   : chassis_control_task.c
  �� �� ��   : V1.0
  ��    ��   : ���ƺ�
  ��������   : 2020.12.10
  ����޸�   : 
  ��������   : �������񣬴�����
  �����б�   : 1) Chassis_Control_Task()		��FreeRTOS����������ϵͳ����������С�
							 2) motor_3508_pid_init()			���ڲ�������Chassis_Control_Task���á�
*******************************************************************************/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "chassis_control_task.h"
#include "pid.h"
#include "encoder.h"
#include "offline_check.h"
#include "motor_use_can.h"

/* �ڲ��궨�� ----------------------------------------------------------------*/

/* �ڲ��Զ����������͵ı��� --------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/
int32_t mini512_cnt = 0;
/* �ڲ�����ԭ������ ----------------------------------------------------------*/
void Motor3508_PID_Init(void);   

/* �������岿�� --------------------------------------------------------------*/
/**
  * @brief				��������
  * @param[out]		
  * @param[in]		
  * @retval				
*/
void Chassis_Control_Task(void const * argument)
{
	int16_t get_speed_z;
	float 	set_speed_z;			  //���ת���趨ֵ
	static  float Current_Value = 0;
	
	Motor3508_PID_Init();//ʹ�����ߵ���ʱ��Ҫ���ú���ע�͵�
	
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		Refresh_Task_OffLine_Time(ChassisControlTask_TOE);
		mini512_cnt = Get_TIM_COUNTER();
		set_speed_z = 500;
		get_speed_z = motor_get[CHASSIS_3508MOTOR].speed_rpm;//���濴��ʱ��Ϊ��
		PID_Calc(&motor_pid[PID_CHASSIS_3508MOTOR_SPEED], get_speed_z, set_speed_z);
		Current_Value = (motor_pid[PID_CHASSIS_3508MOTOR_SPEED].pos_out);
	
		Chassis_Motor3508(&hcan1,Current_Value,0,0,0);
		
		
		osDelayUntil(&xLastWakeTime,CHASSIS_PERIOD);		

	}

}

/**
  * @brief				PID��ʼ��������3508��
  * @param[out]		
  * @param[in]		
  * @retval				
*/
void Motor3508_PID_Init(void)   
{
	PID_Param_Init(&motor_pid[PID_CHASSIS_3508MOTOR_SPEED], POSITION_PID, 1000, 3000,
									10.0f, 0.0f, 0.0f);
}

