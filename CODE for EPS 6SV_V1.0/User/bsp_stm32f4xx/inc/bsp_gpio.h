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
#include "user_cfg.h"

#define OUTPUT_REG1(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOE,GPIO_Pin_2 );else GPIO_ResetBits(GPIOE,GPIO_Pin_2 );}
#define OUTPUT_REG2(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_4 );else GPIO_ResetBits(GPIOD,GPIO_Pin_4 );}
#define OUTPUT_REG3(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_1 );else GPIO_ResetBits(GPIOD,GPIO_Pin_1 );}
#define OUTPUT_REG4(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_2 );else GPIO_ResetBits(GPIOD,GPIO_Pin_2 );}
#define OUTPUT_REG5(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_3 );else GPIO_ResetBits(GPIOD,GPIO_Pin_3 );}
#define OUTPUT_UREG1(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOE,GPIO_Pin_4 );else GPIO_ResetBits(GPIOE,GPIO_Pin_4 );}
#define OUTPUT_UREG2(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_5 );else GPIO_ResetBits(GPIOD,GPIO_Pin_5 );}
#define OUTPUT_UREG3(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOD,GPIO_Pin_6 );else GPIO_ResetBits(GPIOD,GPIO_Pin_6 );}
#define OUTPUT_UREG4(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOE,GPIO_Pin_3 );else GPIO_ResetBits(GPIOE,GPIO_Pin_3 );}
#define OUTPUT_PS(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOE,GPIO_Pin_15 );else GPIO_ResetBits(GPIOE,GPIO_Pin_15 );}
#define OUTPUT_HS(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOB,GPIO_Pin_10 );else GPIO_ResetBits(GPIOB,GPIO_Pin_10 );}
#define OUTPUT_DS(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOA,GPIO_Pin_8 );else GPIO_ResetBits(GPIOA,GPIO_Pin_8 );}
#define OUTPUT_PSW(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOC,GPIO_Pin_6 );else GPIO_ResetBits(GPIOC,GPIO_Pin_6 );}
#define OUTPUT_LED(NewState) {if(NewState == ENABLE)  GPIO_SetBits(GPIOA,GPIO_Pin_12 );else GPIO_ResetBits(GPIOA,GPIO_Pin_12 );}
#define OUTPUT_LED_TOGGLE(void) {GPIO_ToggleBits(GPIOA,GPIO_Pin_12);}


void bsp_InitGPIO(void);

void out_fault_en(FunctionalState NewState);
void led_off(void);
void led_on(void);
void led_toggle(void);
#endif

/*******************************************************************************************************/

