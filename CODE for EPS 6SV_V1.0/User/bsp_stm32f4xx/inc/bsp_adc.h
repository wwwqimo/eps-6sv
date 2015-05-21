/*
*********************************************************************************************************
*
*	模块名称 : ADC驱动模块
*	文件名称 : bsp_adc.c
*	版    本 : V1.0
*	说    明 : adc头文件
*	修改记录 :
*		版本号    日期        作者     说明
*		V1.0    2014-01-08   armfly   正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f4xx.h"


/* define ---------------------------------------------------------------------*/
#define ADC1_DR_ADDRESS          ((uint32_t)0x4001204C)
#define NumofConv                 16//测量通道数
#define BufferSize                (NumofConv)

void bsp_InitADC(void);
float GetTemp(uint16_t advalue);

#define VBATDIV                  2

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
