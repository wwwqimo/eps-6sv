/**
*
*
*
*
*/

#ifndef __BSP_MPPT_INCCOND_H__
#define __BSP_MPPT_INCCOND_H__

typedef struct{
	double volt;
	double curr;
	double volt_step;
	double volt_stepmax;
	double p_v_deri_resolution;
	double k1,k2;
	double volt_set;
	double volt_setmax,volt_setmin;
	double ppt_power;
	double ppt_volt,ppt_curr;
}ppt_inccond_t;

void bsp_ppt_inccond_Init(ppt_inccond_t *ppt,double v_setmax,double v_setmin,double v_stepmax,double k,double p_v_deri_res);
void ppt_inccond(ppt_inccond_t *ppt,double volt,double curr,double volt_start);
#endif
/************************************************************************************************/


