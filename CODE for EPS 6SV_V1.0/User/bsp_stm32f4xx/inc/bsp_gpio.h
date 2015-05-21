/*
****************************************************************************************************
**GPIO
**
**
******************************************************************************************************
*/

#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "stm32f4xx.h"

	
void bsp_InitGPIO(void);
void ocp_out_en(uint8_t out_chan,FunctionalState NewState);
void led_off(void);
void led_on(void);
void led_toggle(void);
#endif

/*******************************************************************************************************/

