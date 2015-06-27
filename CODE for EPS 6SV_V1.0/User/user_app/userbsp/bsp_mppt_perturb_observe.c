/**
*
*
*
*
*/
#include "bsp_mppt_perturb_observe.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void bsp_ppt_pnd_Init(ppt_pnd_t *ppt,double v_setmax, double v_setmin, double step,double p_resolution)
{
	ppt->volt_setmax = v_setmax;
	ppt->volt_setmin = v_setmin;
	ppt->volt_step = step;
	ppt->power_resolution = p_resolution;
}

void ppt_pnd(ppt_pnd_t *ppt,double volt, double curr, double volt_start)
{
	double power_det;
	double volt_det;
//	double p_v_deri;
	power_det = volt * curr - ppt->volt * ppt->curr;
	volt_det = volt - ppt->volt;
	if(power_det > 0.06 || power_det < -0.06) ppt->volt_step = 0.2;
	else ppt->volt_step = 0.01;
//	if((power_det < 0.001 && power_det > 0) || (power_det < 0 && power_det > -(0.001)) ) power_det = 0;
//	p_v_deri = power_det/volt_det;
//	if( p_v_deri> 0)
//	{
//		ppt->volt_step = p_v_deri*0.1;
//	}		
//	else
//	{
//		ppt->volt_step = -(p_v_deri*0.1);
//	}


	if(ppt->volt_step > 0.5) ppt->volt_step = 0.5;
	if(power_det > ppt->power_resolution) 
	{
		if(volt_det >= 0) ppt->volt_set += ppt->volt_step;
		else if(volt_det < 0) ppt->volt_set -= ppt->volt_step;
	}
	else if(power_det < -(ppt->power_resolution)) 
	{
		if(volt_det >= 0) ppt->volt_set -= ppt->volt_step;
		else if(volt_det < 0) ppt->volt_set += ppt->volt_step;
	}
	else 
	{
		if(ppt->ppt_power < volt * curr) 
		{
			ppt->ppt_power = volt * curr;
			ppt->ppt_volt = volt;
			ppt->ppt_curr = curr;
		}
	}
	ppt->volt_setmin = volt_start;
	if(ppt->volt_set < ppt->volt_setmin) ppt->volt_set = ppt->volt_setmin;
	if(ppt->volt_set > ppt->volt_setmax) ppt->volt_set = ppt->volt_setmax;
	ppt->volt = volt;
	ppt->curr = curr;
}



/***************************************************************************************************************************/

