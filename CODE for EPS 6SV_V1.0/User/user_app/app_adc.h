/*
**********************************************************************************************************************
**APP ADC
**
**
*****************************************************************************************************************************
*/

#ifndef __APP_ADC_H__
#define __APP_ADC_H__

#include "user_cfg.h"
#include "user_app.h"



void ADCSample(void);
static uint16_t * adc_acquisition_processing(uint16_t *adc_dest);
static eps_hk_adc_t * adc_to_real(uint16_t *adc_uint,eps_hk_adc_t *adc_dest);

static uint16_t  SlidingFilter(uint16_t adc_value,uint16_t adc_count);
#endif
/*************************************************************************************************************************/



