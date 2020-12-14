/*******************************************************************************
                      ��Ȩ���� (C), 2020-,NCUROBOT
 *******************************************************************************
  �� �� ��   : mecaum_calc.h
  �� �� ��   : V1.0
  ��    ��   : NCUERM
  ��������   : 2018��7��
  ����޸�   : 
  ��������   : ���ֽ���
							 ע�����Ľ�
  �����б�   : 1) Chassis_Vector_To_Mecanum_Wheel_Speed()   ���ⲿ���ã�ʹ�ô���
*******************************************************************************/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "mecaum_calc.h"

/* �ڲ��궨�� ----------------------------------------------------------------*/

/* �ڲ��Զ����������͵ı��� --------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/

/* �ڲ�����ԭ������ ----------------------------------------------------------*/

/* �������岿�� --------------------------------------------------------------*/
/**
  * @brief				���ֽ��㣺�ĸ������ٶ���ͨ�������������������
  * @param[out]		wheel_speed: �ĸ������ٶ�
  * @param[in]		Vx_set: �����ٶ�
									Vy_set: �����ٶ�
	                Wz_set: ��ת�ٶ�	                
  * @retval				none
*/
void Chassis_Vector_To_Mecanum_Wheel_Speed(float  wheel[4],const float Vx_set,const float Vy_set,const float Wz_set)
{
			wheel[0] = (-Vx_set + Vy_set + Wz_set);
			wheel[1] = (-(Vx_set + Vy_set - Wz_set));
			wheel[2] = (-(-Vx_set + Vy_set - Wz_set));
			wheel[3] = (Vx_set + Vy_set + Wz_set);	
}