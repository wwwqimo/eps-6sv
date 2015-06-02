/*
***********************************************************************************************************************
**APP MPPT
**
**
************************************************************************************************************************
*/

#ifndef __APP_MPPT_H__
#define __APP_MPPT_H__

#include "user_app.h"
#include "user_cfg.h"
#include "arm_math.h"
#include "bsp_pid.h"

typedef eps_ppt_t PPT_Typedef;

static float ppt_fixed(PPT_Typedef *ppt);
static float ppt_auto(PPT_Typedef *ppt);
void ppt_data_Init(PPT_Typedef *ppt,uint8_t ppt_num);
void ppt_pid_Init(arm_pid_instance_f32 *pid_para, PID_Typedef *pid_var, uint8_t pid_num);
static uint16_t  pptSlidingFilter(uint16_t adc_value, uint16_t adc_count);
static void pptGetValue(uint16_t *adc_uint,PPT_Typedef *adc_dest);

void app_MPPT(void);
#if MPPT_INC_CONDC_SIM_EN > 0u
    void MPPT_SIM(void);
#endif


#endif 
/*****************************************************************************************************************************/

