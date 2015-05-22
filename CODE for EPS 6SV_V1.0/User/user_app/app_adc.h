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
#include "includes.h"

#define ADC_REF          2500   //ADC参考电压
#define ADC_FullScale    4095  //ADC量程
#define AD7490_REF       2500   //AD7490 参考电压
#define AD7490_FullScale 4095  //ad7490量程




/* eps adc
*************************************************************************************
*/
typedef struct eps_hk_adc_s{
	uint16_t c_sv[6];
	uint16_t v_sv[6];
	uint16_t c_sva;
	uint16_t c_bus;
	uint16_t v_bus;
	uint16_t v_5v;
	uint16_t v_3v;
	uint16_t c_reg[5];
	uint16_t c_ureg[4];
	int16_t temp_eps[4];
	int16_t temp_bat[2];
} eps_hk_adc_t;

#define hk_adc_num  32











void ADCSample(void);
static uint16_t * adc_acquisition_processing(uint16_t *adc_dest);
static eps_hk_adc_t * adc_to_real(uint16_t *adc_uint,eps_hk_adc_t *adc_dest);

static uint16_t  SlidingFilter(uint16_t adc_value,uint16_t adc_count);
#endif
/*************************************************************************************************************************/



