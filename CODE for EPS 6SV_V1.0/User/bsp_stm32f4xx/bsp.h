/*
*********************************************************************************************************
*
*	ģ������ : �ײ�����ģ��
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ���
*	 	       Ӧ�ó���ֻ�� #include bsp.h ���ɣ�����Ҫ#include ÿ��ģ��� h �ļ�
*
*	�޸ļ�¼ :
*		�汾��  ����         ����    ˵��
*		v1.0    2012-12-17  Eric2013  ST�̼���V1.0.2�汾��
*       v1.1    2014-03-22  Eric2013  ����ST�̼��⵽V1.3.0�汾
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H

#define STM32_V5
//#define STM32_X4


#include "stm32f4xx.h"

#include <stdio.h>			/* ��Ϊ�õ���printf���������Ա����������ļ� */

/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
//#include "bsp_uart_fifo.h"
#include "bsp_uart.h"
//#include "bsp_led.h"
#include "bsp_timer.h"
#include "bsp_key.h"

#include "bsp_adc.h"
#include "bsp_dac.h"

#include "bsp_gpio.h"
#include "bsp_pwm.h"
#include "bsp_spi.h"



//#include "bsp_spi_flash.h"
//#include "bsp_cpu_flash.h"
//#include "bsp_sdio_sd.h"
//#include "bsp_i2c_gpio.h"
//#include "bsp_eeprom_24xx.h"
//#include "bsp_si4730.h"
//#include "bsp_hmc5883l.h"
//#include "bsp_mpu6050.h"
//#include "bsp_bh1750.h"
//#include "bsp_bmp085.h"
//#include "bsp_wm8978.h"
//#include "bsp_fsmc_sram.h"
//#include "bsp_nand_flash.h"
//#include "bsp_nor_flash.h"
//#include "LCD_RA8875.h"
//#include "LCD_SPFD5420.h"
//#include "bsp_touch.h"
//#include "bsp_camera.h"
//#include "bsp_ad7606.h"
//#include "bsp_gps.h"
//#include "bsp_oled.h"
//#include "bsp_mg323.h"



void bsp_Init(void);
void bsp_DelayUS(uint32_t _ulDelayTime);
void BSP_Tick_Init (void);
static void NVIC_Configuration(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
