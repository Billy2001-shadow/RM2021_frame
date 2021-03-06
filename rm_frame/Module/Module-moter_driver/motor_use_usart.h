#ifndef __MOTOR_USE_USART_H
#define __MOTOR_USE_USART_H

/* 包含头文件 ---------------------------------------------------------------*/
#include "myinclude.h"
/* 本模块向外部提供的宏定义 -------------------------------------------------*/

/* 本模块向外部提供的结构体/枚举定义 ----------------------------------------*/

/* 本模块向外部提供的变量声明 -----------------------------------------------*/

/* 本模块向外部提供的自定义数据类型变量声明 ---------------------------------*/

/* 本模块向外部提供的接口函数原型声明 ---------------------------------------*/
void AX_12A_Init(uint8_t id);
void AX_12A_Set(uint8_t id,uint16_t angle,uint16_t speed);



#endif

