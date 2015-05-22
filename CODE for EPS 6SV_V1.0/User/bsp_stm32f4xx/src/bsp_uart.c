/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��������ģ��    
*	�ļ����� : bsp_uart.c
*	��    �� : V1.0
*	˵    �� : ʵ��printf��scanf�����ض��򵽴���1����֧��printf��Ϣ��USART1
*				ʵ���ض���ֻ��Ҫ���2������:
*				int fputc(int ch, FILE *f);
*				int fgetc(FILE *f);
*				����KEIL MDK������������ѡ������Ҫ��MicorLibǰ��򹳣����򲻻������ݴ�ӡ��USART1��
*	�޸ļ�¼ :
*		�汾��  ����         ����    ˵��
*		v1.0    2012-12-17 stm32f4  ST�̼���V1.0.2�汾��
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
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
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ��CPU��USART1����Ӳ���豸��δ�����жϡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//initialize uart_fifo struct
	uart_fifo_Init(&uart_fifo,UART_BUF_SIZE,&uart_buffer[0]);
	/* ����3 TX = PD8   RX = PD9 */

	/* ��1���� ����GPIO */

	/* �� GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* �� UART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* �� PD8 ӳ��Ϊ USART3_TX */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

	/* �� PD9 ӳ��Ϊ USART3_RX */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* ���� USART Rx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = 115200;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3, ENABLE);		/* ʹ�ܴ��� */
  
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
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
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	/* дһ���ֽڵ�USART3 */
	USART_SendData(USART3, (uint8_t) ch);

	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc��������������ʹ��scanff�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART3);
}
/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
