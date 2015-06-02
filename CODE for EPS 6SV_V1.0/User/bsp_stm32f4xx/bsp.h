/*
*********************************************************************************************************
*
*	模块名称 : 底层驱动模块
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。
*	 	       应用程序只需 #include bsp.h 即可，不需要#include 每个模块的 h 文件
*
*	修改记录 :
*		版本号  日期         作者    说明
*		v1.0    2012-12-17  Eric2013  ST固件库V1.0.2版本。
*       v1.1    2014-03-22  Eric2013  升级ST固件库到V1.3.0版本
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H

#define STM32_V5
//#define STM32_X4


#include "stm32f4xx.h"

#include <stdio.h>			/* 因为用到了printf函数，所以必须包含这个文件 */

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
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

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
