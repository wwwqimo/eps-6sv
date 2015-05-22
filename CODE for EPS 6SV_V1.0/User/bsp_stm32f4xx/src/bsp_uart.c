/*
*********************************************************************************************************
*	                                  
*	模块名称 : 串口驱动模块    
*	文件名称 : bsp_uart.c
*	版    本 : V1.0
*	说    明 : 实现printf和scanf函数重定向到串口1，即支持printf信息到USART1
*				实现重定向，只需要添加2个函数:
*				int fputc(int ch, FILE *f);
*				int fgetc(FILE *f);
*				对于KEIL MDK编译器，编译选项中需要在MicorLib前面打钩，否则不会有数据打印到USART1。
*	修改记录 :
*		版本号  日期         作者    说明
*		v1.0    2012-12-17 stm32f4  ST固件库V1.0.2版本。
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "stm32f4xx.h"
#include <stdio.h>

#include "includes.h"
#include <os_cpu.h>
extern void UartReciveNew(void);
//declaration uart_fifo variable;
uart_fifo_t uart_fifo;
//declaration uart_buffer[] array variable
uint8_t uart_buffer[UART_BUF_SIZE];

static void NVIC_UART3_Config(void);
//uart buffer initialize
void uart_fifo_Init(uart_fifo_t *uart_fifo,uint16_t size,uint8_t * buf)
{
	uart_fifo->buf = buf;
	uart_fifo->size = size;
	uart_fifo->flag = 0;
	uart_fifo->free = size;
	uart_fifo->getP = 0;
	uart_fifo->putP = 0;
	uart_fifo->ReciveNew = UartReciveNew;
}
//buffer add a data
int32_t uart_fifo_put(uart_fifo_t *uart_fifo,uint8_t data)
{
	if(uart_fifo->free == 0)
	{
		uart_fifo->flag |= UART_OVERFLOW;
		return (-1);
	}
	uart_fifo->buf[uart_fifo->putP] = data;
	uart_fifo->putP++;
	//cycle buffer 
	if(uart_fifo->putP == uart_fifo->size)
	{
		uart_fifo->putP = 0;
	}
	uart_fifo->free--;
	return 0;
}
//get a data from buffer
int32_t uart_fifo_get(uart_fifo_t *uart_fifo)
{
	int32_t data;
	if(uart_fifo->free == uart_fifo->size)
	{
		return (-1);
	}
	data = uart_fifo->buf[uart_fifo->getP];
	uart_fifo->getP++;
	//cycyle buffer
	if(uart_fifo->getP == uart_fifo->size)
	{
		uart_fifo->getP = 0;
	}
	uart_fifo->free++;
	return data;
}

// buffer used size
int32_t uart_fifo_used(uart_fifo_t *uart_fifo)
{

	return uart_fifo->size - uart_fifo->free;
}
//buffer free size
int32_t uart_fifo_free(uart_fifo_t *uart_fifo)
{
	return uart_fifo->free;
}
	//clear the uart buffer;
void uart_fifo_clear(uart_fifo_t *uart_fifo)
{
	uart_fifo->free = UART_BUF_SIZE;
	uart_fifo->getP = 0;
	uart_fifo->putP = 0;
}
//uart interruot service 
void USART3_IRQHandler(void)
{
	uint16_t rxdata;
	#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0u;
	#endif
	OS_ENTER_CRITICAL();
	if(USART_GetITStatus(USART3, USART_IT_RXNE)!=RESET)
	{
		rxdata = USART_ReceiveData(USART3);
//		printf("uart get message %c", (unsigned char)rxdata);
		uart_fifo_put(&uart_fifo, rxdata);

		if((char)rxdata == '\r'||(char)rxdata == '\n')
		{
			printf("\r\n");
			uart_fifo.ReciveNew();
		}
		else
		{
			printf("%c",(char)rxdata);
		}
	}
	 OS_EXIT_CRITICAL();
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化CPU的USART1串口硬件设备。未启用中断。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//initialize uart_fifo struct
	uart_fifo_Init(&uart_fifo,UART_BUF_SIZE,&uart_buffer[0]);
	/* 串口3 TX = PD8   RX = PD9 */

	/* 第1步： 配置GPIO */

	/* 打开 GPIO 时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* 打开 UART 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* 将 PD8 映射为 USART3_TX */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

	/* 将 PD9 映射为 USART3_RX */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	/* 配置 USART Tx 为复用功能 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* 输出类型为推挽 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 内部上拉电阻使能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* 复用模式 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* 配置 USART Rx 为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = 115200;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3, ENABLE);		/* 使能串口 */
  
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	NVIC_UART3_Config();	
}
void NVIC_UART3_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	/* 写一个字节到USART3 */
	USART_SendData(USART3, (uint8_t) ch);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用scanff函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART3);
}
/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
