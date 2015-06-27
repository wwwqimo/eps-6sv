/**
*
*
*
*
*/


#ifndef __BSP_MPPT_PERTURB_OBSERVE_H__
#define __BSP_MPPT_PERTURB_OBSERVE_H__




typedef struct{
	double volt,curr;
	double power;
	double power_resolution;
	double volt_step;
	double volt_set;
	double volt_setmax,volt_setmin;
	double ppt_power;
	double ppt_volt,ppt_curr;
}ppt_pnd_t;


void bsp_ppt_pnd_Init(ppt_pnd_t *ppt,double v_setmax, double v_setmin, double step,double p_resolution);
void ppt_pnd(ppt_pnd_t *ppt,double volt, double curr, double volt_start);







#endif

/*********************************************************************************************************************************/


