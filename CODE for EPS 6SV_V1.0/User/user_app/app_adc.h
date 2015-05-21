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

typedef struct eps_hk_state_s{
	uint8_t sv_state[6];    //eps solar input state
	uint8_t conv_state[2];  //eps regulator converter state
	uint8_t reg_state[5];   //eps regulate ouputs states
	uint8_t ureg_state[4];  //eps unregulate outputs state
	uint16_t out_fault;     //eps output faults count number 
	uint16_t reg_fault[5];  //eps every regulate output fault count number
	uint16_t ureg_fault[4]; //eps every unregulate output fault count number
	uint16_t eps_power_mode;        //eps power select
} eps_hk_state_t;
#define hk_adc_num  32



void ADCSample(void);
uint16_t * adc_acquisition_processing(uint16_t *adc_dest);
eps_hk_adc_t * adc_to_real(uint16_t *adc_uint,eps_hk_adc_t *adc_dest);

uint16_t  SlidingFilter(uint16_t adc_value,uint16_t adc_count);
#endif
/*************************************************************************************************************************/



