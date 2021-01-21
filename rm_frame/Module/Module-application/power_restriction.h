#ifndef __POWER_RESTRICTION_H
#define __POWER_RESTRICTION_H
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "myinclude.h"



/* ��ģ�����ⲿ�ṩ�ĺ궨�� --------------------------------------------------*/

/* ��ģ�����ⲿ�ṩ���������Ͷ��� --------------------------------------------*/
typedef struct{
	
 float  CurrentBuff1_get; 			 		//��һ���˲����
 float  CurrentBuff2_fliter[11];		//�ڶ����˲�����
 float  CurrentBuff2_get;					  //�ڶ����˲����
 float  CurrentCalculat;			 			//����������
 float  Current_Offset;						  //����ƫ����(�Ͳ���ϵͳ�Ƚ�)
 float  Current_Referee;						//���Բ���ϵͳ�ĵ���ֵ
int16_t  Current_Offset_num;

}Current_GET;   //�����ɼ�

typedef struct{
		
 float  Volt_Referee;								//���Բ���ϵͳ�ĵ�ѹֵ
 float  Power_Referee;							//���Բ���ϵͳ�Ĺ���
 float  Power_Referee_Last;         
 float  Power_Calculat;							//���м���Ĺ���ֵ
 float  PowerRemain_Referee;				//���Բ���ϵͳ��ʣ�໺������
 float  PowerRemain_Referee_last;		//���Բ���ϵͳ����һ��ʣ�໺������
 float  PowerRemain_Calculat;				//���м���Ĳ���ϵͳ��ʣ�໺������
 float  PowerRemain_Calculat_Last;  //���м������һ�β���ϵͳ��ʣ�໺������
 float  PowerRemain_Calculat_Next;
 float  PowerLimit;									//��������ֵ
 
}Limit;				 //��������




typedef struct{

	uint32_t time_now;
	uint32_t time_last;
	int32_t time;
	int32_t total_time_ms;
	double total_time_s;
	
}MyTimeTick;
/* ��ģ�����ⲿ�ṩ�Ľӿڳ������� --------------------------------------------*/
extern  uint32_t  uhADCxConvertedValue[10];  //ADC��������
extern  Current_GET  current_get;
extern  Limit  limit;
extern  MyTimeTick  time_for_limit;
/* ��ģ�����ⲿ�ṩ�Ľӿں���ԭ������ ----------------------------------------*/
void power_limit(float  Current_get[2]);
float Limit_filter(float oldData,float newData,float val);

#endif
