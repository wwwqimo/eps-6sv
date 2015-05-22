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




/* ppt converter struct
********************************************************
*/
typedef struct{
	uint16_t curr;        //hte current input current[mA]
	uint16_t volt;        //the current input voltage[mv]
	uint16_t out_volt;    //the output voltage [mv]
	uint8_t mode;         //mode for ppt[0 = fixed, 1 = auto]
	uint16_t ppt_volt;    //fixed ppt point for converters [mv],only when ppt_mode = 0,it is useful
} eps_ppt_t;
















static uint16_t  pptSlidingFilter(uint16_t adc_value,uint16_t adc_count);
static void pptGetValue(uint16_t *adc_uint,eps_ppt_t *adc_dest);

void app_MPPT(void);
#if MPPT_INC_CONDC_SIM_EN > 0u
    void MPPT_SIM(void);
#endif


#endif 
/*****************************************************************************************************************************/

