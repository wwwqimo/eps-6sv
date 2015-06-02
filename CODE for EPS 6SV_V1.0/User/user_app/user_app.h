#ifndef __USER_APP_H__
#define __USER_APP_H__


#include "bsp.h"
#include "user_bsp.h"

#include "user_app.h"
#include "user_print.h"
#include "eps_handle.h"
#include "app_adc.h"
#include "app_mppt.h"

#include <stdio.h>
#include <string.h>


extern eps_hk_adc_t eps_adc_data;
//定义全局变量 bat结构体，ppt结构体
extern eps_bat_t eps_bat;	
//define eps hk state struct avariable
extern eps_hk_state_t eps_state;
extern eps_ppt_t ppt_conv[SV_NUM];
//声明 eps_adc_data结构体

extern uart_fifo_t uart_fifo;

void user_app_Init(void);
void user_data_handling(void);
#endif
/*********************************************************************************************************************************/

