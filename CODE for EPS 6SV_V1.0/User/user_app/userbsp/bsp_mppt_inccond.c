/**
*
*
*
*
*
*/
#include "bsp_mppt_inccond.h"
#include "bsp.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


void bsp_ppt_inccond_Init(ppt_inccond_t *ppt,double v_setmax,double v_setmin,double v_stepmax,double k,double p_v_deri_res)
{
	ppt->k1 = 4*k;
	ppt->k2 = k;
	ppt->volt_stepmax = v_stepmax;
	ppt->volt_setmax = v_setmax;
	ppt->volt_setmin = v_setmin;
	ppt->p_v_deri_resolution = p_v_deri_res;
}

void ppt_inccond(ppt_inccond_t *ppt,double volt,double curr,double volt_start)
{
	double power_det;
	double volt_det;
	double p_v_deri;
	double k;
	ppt->volt_setmin = volt_start;
	volt_det = volt - ppt->volt;
	power_det = volt * curr - ppt->curr * ppt->volt;
	p_v_deri = power_det/volt_det;
	if(volt_det != 0)
	{
		if(power_det > 0.02 || power_det < -(0.02))
		{
			if(p_v_deri > ppt->p_v_deri_resolution || p_v_deri < -(ppt->p_v_deri_resolution))
			{
				if(p_v_deri > 0)
				{
					k = ppt->k1;
				}
				else
				{
					k = ppt->k2;
				}
			}
			else
			{
				k = 0.1*ppt->k1;
			}
			ppt->volt_step = k * p_v_deri;
			if(ppt->volt_step > ppt->volt_stepmax) ppt->volt_step = ppt->volt_stepmax;
			else if(ppt->volt_step < -(ppt->volt_stepmax)) ppt->volt_step = -(ppt->volt_stepmax);
			ppt->volt_set += ppt->volt_step;
		}
		else
		{
			ppt->volt_set = 0.5*(volt + ppt->volt);
			if(ppt->ppt_power < volt * curr)
			{
				ppt->ppt_power = volt * curr;
				ppt->ppt_volt = volt;
				ppt->ppt_curr = curr;
			}
		}
	}
	if(ppt->volt_set >= ppt->volt_setmax) ppt->volt_set = ppt->volt_setmax;
	if(ppt->volt_set <= ppt->volt_setmin) ppt->volt_set = ppt->volt_setmin;
	
	ppt->volt = volt;
	ppt->curr = curr;
	
}
/***********************************************************************************************************/

