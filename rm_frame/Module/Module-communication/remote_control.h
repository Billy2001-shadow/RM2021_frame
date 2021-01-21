#ifndef __RC_H
#define __RC_H
/* ����ͷ�ļ� ---------------------------------------------------------------*/
#include "myinclude.h"

/* ��ģ�����ⲿ�ṩ�ĺ궨�� -------------------------------------------------*/
#define DR16_RX_BUFFER_SIZE 36u
#define DR16_DATA_LEN 18u

/* ��ģ�����ⲿ�ṩ�Ľṹ��/ö�ٶ��� ----------------------------------------*/
//ң�������ݽṹ��
typedef __packed struct
{
	__packed struct
	{
			int16_t ch0;
			int16_t ch1;
			int16_t ch2;
			int16_t ch3;
			uint8_t s1;
		  uint8_t s2;
	} rc;
	__packed struct
	{
			int16_t x;
			int16_t y;
			int16_t z;
			uint8_t press_l;
			uint8_t press_r;
	} mouse;
	__packed struct
	{
			uint16_t v;
	} key;

} RC_ctrl_t;

/* ��ģ�����ⲿ�ṩ�ı������� -----------------------------------------------*/

/* ��ģ�����ⲿ�ṩ���Զ����������ͱ������� ---------------------------------*/
extern RC_ctrl_t rc_ctrl;//ң�������ݽṹ��


/* ��ģ�����ⲿ�ṩ�Ľӿں���ԭ������ ---------------------------------------*/
void Remote_Control_USART_Init(void);
void DR16_UART_IRQHandler(void);

#endif
