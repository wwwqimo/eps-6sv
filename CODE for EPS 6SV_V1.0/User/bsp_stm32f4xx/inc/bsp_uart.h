/*
*********************************************************************************************************
*	                                  
*	模块名称 : 串口驱动模块    
*	文件名称 : bsp_uart.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_UART_H
#define __BSP_UART_H


#include "stm32f4xx.h"

#define UART_BUF_SIZE 1024
#define UART_OVERFLOW 0X0001;
typedef struct{
	uint8_t * buf;
	int32_t putP;
	int32_t getP;
	uint32_t size;
	uint32_t free;
	int32_t flag;
	void (*ReciveNew)(void);
}uart_fifo_t;

void uart_fifo_Init(uart_fifo_t *uart_fifo,uint16_t size,uint8_t * buf);
int32_t uart_fifo_put(uart_fifo_t *uart_fifo,uint8_t data);
int32_t uart_fifo_get(uart_fifo_t *uart_fifo);
int32_t uart_fifo_delete(uart_fifo_t *uart_fifo);
int32_t uart_fifo_used(uart_fifo_t *uart_fifo);
int32_t uart_fifo_free(uart_fifo_t *uart_fifo);
void uart_fifo_clear(uart_fifo_t *uart_fifo);
void bsp_InitUart(void);
void USART3_IRQHandler(void);
#endif


