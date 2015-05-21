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
void NVIC_UART3_Config(void);
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
