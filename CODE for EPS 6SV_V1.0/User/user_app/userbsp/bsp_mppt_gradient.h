/**
*
*
*
*
*
*/
#ifndef __BSP_MPPT_GRADIENT__
#define __BSP_MPPT_GRADIENT__

#include "stm32f4xx.h"
#include "bsp.h"

typedef struct{
	double volt;//volt是上一次电压值
	double curr;//curr上次电流值
	double volt_step,volt_step_max,volt_step_min;//当前电压步长及其范围
	double p_v_deri_resolution;//功率对电压导数 dP/dU
	double step_k; //step计算因子
	double volt_set;  //下次电压设定值
	double power_ppt,volt_ppt,curr_ppt;  //功率点状态量
}ppt_gradient_t;


void bsp_ppt_gradient_Init(ppt_gradient_t *ppt,double v_stepmax,double v_stepmin,double p_deri_resolution);
void ppt_gradient(ppt_gradient_t *ppt,double volt, double curr, double volt_start);
#endif

/********************************************************************************************/

