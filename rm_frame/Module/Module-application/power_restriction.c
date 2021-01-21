/*******************************************************************************
                      ��Ȩ���� (C), 2017-,NCUROBOT
 *******************************************************************************
  �� �� ��   : power_restriction.c
  �� �� ��   : ����
  ��    ��   : NCUERM
  ��������   : 2018��7��
  ����޸�   :
  ��������   : ���̹�������
  �����б�   :void power_limit(float  Current_get)


*******************************************************************************/
/*
* ��ѹ������������Ĺ�ϵ:
*			VOUT = VOUT(Q) + Vsens*I;
*	VOUT(Q)Ϊ��̬�����ѹ����VCC/2��Ҳ����������ʱ������ĵ�ѹ��Vsens�Ǵ�����������
*��ʹ�õ��ͺŵ�ϵ����40MV/A��I�ķ����Ǵ�IP+����IP-�ĵ���
*	eg:
*			VCCΪ5V��I����Ϊ10A����ô�����Ϊ5V / 2 + 40MV/A * 10A = 2.9V
*			VCCΪ3.3V��I����Ϊ10A����ô�����Ϊ3.3V / 2 + 40MV/A*10A = 2.05V
* �����
*			I = (VOUT - VOUT(Q))/Vsens
*			I = (V_get - 2.5)/0.04; 	  //��5V��ѹ
*			I = (V_get - 1.65)/0.04;		//��3.3V��ѹ
*/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "power_restriction.h"
#include "referee.h"
#include "user_lib.h"
/* �ڲ��Զ����������� --------------------------------------------------------*/

/* �ڲ��궨�� ----------------------------------------------------------------*/
#define mylimit 20
#define mylimitpower 200
#define MyAbs(x) ((x > 0) ? (x) : (-x))
#define VAL_LIMIT(val, min, max)\
if(val<=min)\
{\
	val = min;\
}\
else if(val>=max)\
{\
	val = max;\
}\

//#define Predict_Time  0.05fb
#define Predict_RemainPower 20
/* ���������Ϣ����-----------------------------------------------------------*/

/* �ڲ���������---------------------------------------------------------------*/

/* �ⲿ�������� --------------------------------------------------------------*/

/* �ⲿ����ԭ������ ----------------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/
/*
*
*	����˵��:ADC�������� , ����ϵ��0.8058608{3300/4095}
*  			  �õ�ʵ�ʵ�ѹֵ
*	����ʱ��:�ɼ�һ�����ݵ�ʱ����(ת������+��������)/ʱ��(12λ�ɼ���15������)
*				  �������õĲ���ʱ����(12+3)/(84/4)M = 0.7142us
*	   Note:ʵ�ʵ������ݵļ����С5ms�����Դ���һ������Ϊ10�����飬ÿ��ʹ�����ݵ�ʱ��
*		   		��10������ȡƽ��ֵ��Ȼ���ٴ���һ��10λ������洢��ʷ���ݣ����д��ڻ�����
*					�� (��Ϊ���ڻ�û��Ӳ����ƣ���ʱ�����ַ������˲�),������Чʱ���ӳ���0.7ms
*/
uint32_t  uhADCxConvertedValue[10] = {0};  
Limit  limit = {.PowerRemain_Calculat = mylimitpower};
Current_GET  current_get = {0};
MyTimeTick  time_for_limit = {0}; //����ʱ���ʱ
MyTimeTick  time_for_RP = {0};  //remain power �������������ʱ
//int16_t angle1[4];

//���Ա���
int limit_flag = 0;
/* ����ԭ������ ----------------------------------------------------------*/

/*
** Descriptions: ���Բ�������ʱ��(s)
** Input: ʱ��ṹ���ָ��
** Output: NULL
*/
void	MyTime_statistics(MyTimeTick *time_tick)
{

	time_tick->time_now = HAL_GetTick();
	if(time_tick->time_last == 0)//�����һ�μ�����ʱ��lastΪ��
	{
		time_tick->time = time_tick->time_now - time_tick->time_now;
	}else
	{
		time_tick->time = time_tick->time_now - time_tick->time_last;		
	}
	time_tick->time_last = time_tick->time_now;
	//�м�������ʱ��
	time_tick->total_time_ms += time_tick->time;
	time_tick->total_time_s = time_tick->total_time_ms * 0.001f;
}
/*
** Descriptions: ���ʱ��ṹ��
** Input: 
**				MyTimeTick *: 		ʱ��ṹ���ָ��
**				flag:  ѡ����յ�����
**								flag = 1  ���ȫ��
**								flag = 2	�����ʱ��(��ֹ��������ʱ�ļ�ʱ�������)
** Output: NULL
*/
void MyTime_memset(MyTimeTick *time_tick ,char flag)
{
	if(flag == 1)
	{
		memset(time_tick,0,sizeof(MyTimeTick));
	}else if(flag == 2)
	{
		time_tick->total_time_ms = 0;
		time_tick->total_time_s = 0;
	}
	
}

/*
** Descriptions: ADC���ݲɼ����˲�
** Input: NULL
** Output: ���ݲɼ�ֵ
*/
void Get_ADC_Value(void)
{
		uint32_t *buff = uhADCxConvertedValue;
		//��һ���˲�Ҫ�ر�ADC��dma
		int getbuff = 0;
		for(uint8_t i = 0;i < 10;i++)
		{
			getbuff += buff[i];
		}
		/*current_get.CurrentBuff1_get =  Median_value_fliter(buff,10);
			current_get.CurrentBuff1_get =  Median_average_fliter(buff,10);*/
		current_get.CurrentBuff1_get =  getbuff * 0.1f;
		current_get.CurrentBuff2_fliter[10] = current_get.CurrentBuff1_get;
		current_get.CurrentBuff2_get = Window_sliding_filter(current_get.CurrentBuff2_fliter)*0.1f;
	
		if(current_get.Current_Offset_num > 200)
		{
			
			current_get.CurrentCalculat = (current_get.CurrentBuff2_get * (0.00080566f) - 2.50f) * 25.0f -
																		 current_get.Current_Offset;
		}
		else
		{
			current_get.Current_Offset_num++;
			current_get.CurrentCalculat = (current_get.CurrentBuff2_get * (0.00080566f) - 2.50f) * 25.0f;
			
			if(current_get.Current_Offset_num > 50)
			{		
				current_get.Current_Offset += current_get.CurrentCalculat - current_get.Current_Referee;
			}
			if(current_get.Current_Offset_num > 200)
			{				
				current_get.Current_Offset = current_get.Current_Offset/150.0f;
			}
		}

}

/*
** Descriptions: ������������
** Input: NULL
** Output: NULL
*/

void Remain_Power_Calculate(void)
{
		/*�ɼ�ʱ��*/
		MyTime_statistics(&time_for_RP);
	
		/*�����������*/
		if(limit.Power_Calculat >80)
		{
			limit.PowerRemain_Calculat -= (limit.Power_Calculat - 80) * time_for_RP.time * 0.01f;
		}
		if(limit.Power_Calculat <80)
		{
			limit.PowerRemain_Calculat = 60-(limit.Power_Calculat - 80) * time_for_RP.time * 0.01f;
		}
		
		/*�����ʱ��*/
		MyTime_memset(&time_for_RP,2);		
		
		/*�ָ���������*/
		VAL_LIMIT(limit.PowerRemain_Calculat, 0.0f, 60.0f);

}

/*
** Descriptions: ��������
** Input: NULL
** Output: ��������ֵ
*/

/*
*	��γ�����ò���ϵͳ���صĹ��ʺ���������
*
*/
void power_limit(float  Current_get[2])
{
		
//		float total_current = 0;
	

////		total_current = MyAbs(Current_get[0]) + MyAbs(Current_get[1]); 
//		total_current = MyAbs(Current_get[0]); 
		
//		/*�����ɼ�*/
//		Get_ADC_Value();

		/*���ʼ���*/
//		if(limit.Volt_Referee != 0)//��ֹ����ϵͳʧЧ
//		{
//			limit.Power_Calculat = current_get.CurrentCalculat * limit.Volt_Referee;

//		}else
//		{
//  			limit.Power_Calculat = current_get.CurrentCalculat * 23.0f;		
//		}
		
		/*�����������*/
//		Remain_Power_Calculate();
		
		/*��������*/
		//	limit.PowerRemain_Calculat_Last=limit.PowerRemain_Calculat;
//		limit.PowerRemain_Calculat_Last = referee.power_heat_data.chassis_power_buffer;
//		limit.Power_Calculat = referee.power_heat_data.chassis_power;
		
				

//			if(limit.Power_Calculat - 20 <=0)
//			{
//				limit.Power_Calculat  = 20;
//			}
//			
//			if(((limit.PowerRemain_Calculat_Last - (limit.Power_Calculat - 20) * 0.2 ) < Predict_RemainPower)||(limit_flag==1))
//			{
//				limit_flag = 1;
//				limit.PowerLimit = 600 + 0.102 * (limit.PowerRemain_Calculat_Last * limit.PowerRemain_Calculat_Last);//0.1
//				
//			/*��������*/
//				Current_get[0] = (Current_get[0]/(total_current + 1.0f)) * limit.PowerLimit; 
//		//		Current_get[1] = (Current_get[1]/(total_current + 1.0f)) * limit.PowerLimit; 

//			}
//		 if(limit.PowerRemain_Calculat_Last >= 190)
//			{
//				limit_flag = 0;	  				
//			}
//			else limit.PowerLimit = 0;
}

