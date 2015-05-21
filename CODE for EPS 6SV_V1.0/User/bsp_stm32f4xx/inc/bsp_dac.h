/*
*********************************************************************************************************
*
*	ģ������ : DAC����ģ��
*	�ļ����� : bsp_dac.c
*	��    �� : V1.0
*	˵    �� : ʵ��DAC�������
*	�޸ļ�¼ :
*		�汾��    ����        ����     ˵��
*		V1.0    2013-12-30   armfly   ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_DAC_H
#define __BSP_DAC_H

#include "stm32f4xx.h"

void bsp_InitDAC(void);
void TIM6_Config(void);
void DAC_Ch1_TriangleConfig(void);
void DAC_Ch1_AmplitudeConfig(uint32_t _DAC_Amplitude);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
