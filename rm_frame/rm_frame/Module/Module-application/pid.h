#ifndef  __PID_H
#define  __PID_H

/* ����ͷ�ļ� ---------------------------------------------------------------*/
#include "myinclude.h"
/* ��ģ�����ⲿ�ṩ�ĺ궨�� -------------------------------------------------*/

/* ��ģ�����ⲿ�ṩ�Ľṹ��/ö�ٶ��� ----------------------------------------*/

enum PID_MODE
{
		POSITION_PID =0,
		DELTA_PID = 1,
};

enum Time
{
    LLAST	= 0,//����һ��
    LAST 	= 1,//��һ��
    NOW 	= 2,//����һ��
};

typedef enum
{
	PID_CHASSIS_3508MOTOR_SPEED = 0,
	PID_CHASSIS_3508MOTOR_POS = 1,
	PID_TRIGGER_2006MOTOR_SPEED = 2,
	PID_TRIGGER_2006MOTOR_POS = 3,
	PID_YAW_3508MOTOR_ID_SPEED = 4,
	PID_YAW_3508MOTOR_ID_POS = 5,
	PID_PITCH_6020MOTOR_ID_SPEED = 6,
	PID_PITCH_6020MOTOR_ID_POS = 7,
	
	NUM_OF_PID = 8,//���е������PID����ʱ����PID�ṹ�������
	
}PID_ID;//����ջ�����ʱ������ٶȻ���λ�û�PID�ṹ���Ӧ�洢��������


typedef struct __pid_t
{
	  uint8_t pid_mode;       	//PIDģʽ

		//PID������
    float Kp;			
    float Ki;
    float Kd;

    float set[3];							//Ŀ��ֵ,����NOW�� LAST��LLAST
    float get[3];							//����ֵ
    float err[3];							//���

    float pout;								//p���
    float iout;								//i���
    float dout;								//d���
    float last_dout;					//��һ��d���
	
		float max_pout;						//�����޷�
		float max_iout;						//�����޷�
		float max_dout;						//΢���޷�
		float max_out;						//����޷�		
		float max_err;						//������
    float deadband;						//���� 

    float pos_out;						//����λ��ʽ���
    float last_pos_out;				//�ϴ�λ��ʽ���
    float delta_out;					//��������ʽ���
    float last_delta_out;			//�ϴ�����ʽ���

} pid_t;
/* ��ģ�����ⲿ�ṩ�ı������� -----------------------------------------------*/

/* ��ģ�����ⲿ�ṩ���Զ����������ͱ������� ---------------------------------*/
extern pid_t   motor_pid[NUM_OF_PID];

/* ��ģ�����ⲿ�ṩ�Ľӿں���ԭ������ ---------------------------------------*/
void PID_Param_Init(pid_t *pid, uint8_t mode, float max_out, float max_iout, float Kp, float Ki, float Kd);
float PID_Calc(pid_t* pid, float get, float set);
void PID_Clear(pid_t *pid);



#endif

