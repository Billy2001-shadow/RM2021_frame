/*******************************************************************************
                      ��Ȩ���� (C), 2020-,NCUROBOT
 *******************************************************************************
  �� �� ��   : vision.c
  �� �� ��   : V1.0
  ��    ��   : ���ƺ�
  ��������   : 2020.12.8
  ����޸�   : 
  ��������   : �������ݽ�������뼰���͡�USART��ʼ����UASRT�жϴ������ݽ��롢���ݷ��͡�
  �����б�   : 1) VisionDate_Receive_Init()  ���ⲿ���ã�bsp.c��
							 2) Vision_UART_IRQHandler()	 ���ⲿ���ã�stm32f4xx_it.c��USART7�жϷ�������
							 3) SBUS_To_Vision()					 ���ڲ����ã�Vision_UART_IRQHandler()��
*******************************************************************************/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "vision.h"
#include "bsp_usart.h"

/* �ڲ��궨�� ----------------------------------------------------------------*/
#define VD_huart  huart7
/* �ڲ��Զ����������͵ı��� --------------------------------------------------*/
Minipc_Rx_Struct minipc_rx; //��������minipc������

/* �ڲ����� ------------------------------------------------------------------*/
/*static */uint8_t vision_rx_buf[VISION_RX_BUFFER_SIZE];//�����Ӿ�ԭʼ���ݵ�����
uint16_t vision_rx_date_len;//ʵ�ʽ��յ��Ӿ����ݵĳ���

/* �ڲ�����ԭ������ ----------------------------------------------------------*/
static void SBUS_To_Vision(volatile const uint8_t *buff, Minipc_Rx_Struct *Minipc_Rx);

/* �������岿�� --------------------------------------------------------------*/
/**
  * @brief				�Ӿ����ݽ���USART�����жϳ�ʼ������
  * @param[out]		
  * @param[in]		
  * @retval				
*/
void VisionDate_Receive_USART_Init(void)
{
    UART_IT_Init(&VD_huart,vision_rx_buf,VISION_RX_BUFFER_SIZE);
}


/**
  * @brief				�Ӿ����ݽ��յĴ����Ŀ����жϺ��жϴ�����
  * @param[out]		
  * @param[in]		
  * @retval				
*/
uint32_t Vision_UART_IRQHandler(void)
{
	static  BaseType_t  pxHigherPriorityTaskWoken;

	if (__HAL_UART_GET_FLAG(&VD_huart, UART_FLAG_IDLE))
	{
			/* clear idle it flag avoid idle interrupt all the time */
			__HAL_UART_CLEAR_IDLEFLAG(&VD_huart);

			/* clear DMA transfer complete flag */
			__HAL_DMA_DISABLE(VD_huart.hdmarx);

			//��������Ӿ����ݵĳ���
			vision_rx_date_len = (VISION_RX_BUFFER_SIZE - VD_huart.hdmarx->Instance->NDTR);
			//���ݽ���
			SBUS_To_Vision(vision_rx_buf,&minipc_rx);
			//����֪ͨ
			vTaskNotifyGiveFromISR(VisionDataTaskHandle,&pxHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);	

			/* restart dma transmission */
			__HAL_DMA_SET_COUNTER(VD_huart.hdmarx, VISION_RX_BUFFER_SIZE);
			__HAL_DMA_ENABLE(VD_huart.hdmarx);				
			
	}
	return 0;
}

/**
  * @brief				�Ӿ����ݽ��뺯��
  * @param[out]		
  * @param[in]		
  * @retval				
*/
static void SBUS_To_Vision(volatile const uint8_t *buff, Minipc_Rx_Struct *Minipc_Rx)
{
	if (buff == NULL || Minipc_Rx == NULL)
	{
			return;
	}
	
	Minipc_Rx->frame_header = buff[0];
	Minipc_Rx->frame_tail 	= buff[8];
	
	if((Minipc_Rx->frame_header == 0xFF) && (Minipc_Rx->frame_tail == 0xFE))
	{
		Minipc_Rx->angle_yaw  = (int16_t)(buff[1] << 8 | buff[2]);
		Minipc_Rx->angle_pit  = (int16_t)(buff[3] << 8 | buff[4]);
		Minipc_Rx->state_flag = buff[5];
		Minipc_Rx->distance   = buff[6]<<8|buff[7];
	}
	
}


