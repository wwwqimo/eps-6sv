#include "bsp_mppt_gradient.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
/**
*/
void bsp_ppt_gradient_Init(ppt_gradient_t *ppt,double v_stepmax,double v_stepmin,double p_deri_resolution)
{
	ppt->volt_step_max = v_stepmax;
	ppt->volt_step_min = v_stepmin;
	ppt->step_k = 0.1;
	ppt->p_v_deri_resolution = p_deri_resolution;
}
/**
*/
//void ppt_gradient(ppt_gradient_t *ppt,double volt, double curr, double volt_start)
//{
//	
//}
void ppt_gradient(ppt_gradient_t *ppt,double volt, double curr, double volt_start)
{
	double volt_det;
	double curr_det;
  double power_det;
	double p_v_deri; //功率对电压导数
	
	volt_det = volt - ppt->volt;
	curr_det = curr - ppt->curr;
	power_det = curr * volt - ppt->volt * ppt->curr;
//	/*计算dP/dU*/
//	p_v_deri = power_det/volt_det;
	p_v_deri = curr + curr_det/volt_det;
	if(p_v_deri > 0.005 || p_v_deri < -0.005)
	{/*大于0，工作点在功率点的左侧，增大调控电压*/ /*小于0，工作点在功率点右侧,减小输出电压*/
		ppt->volt_step = ppt->step_k * p_v_deri;
		/*volt_step 数据范围限定*/
		if(ppt->volt_step > ppt->volt_step_max) ppt->volt_step = ppt->volt_step_max;
//		else if(ppt->volt_step < ppt->volt_step_min && ppt->volt_step > 0) ppt->volt_step = ppt->volt_step_min;
//		else if(ppt->volt_step > -(ppt->volt_step_min ) && ppt->volt_step < 0) ppt->volt_step = -(ppt->volt_step_min);
		else if(ppt->volt_step < -(ppt->volt_step_max)) ppt->volt_step = -(ppt->volt_step_max);
		
		ppt->volt_set += ppt->volt_step;
		/*设定点最小值判断*/
		if(ppt->volt_set < volt_start) ppt->volt_set = volt_start;
		if(ppt->volt_set > 15) ppt->volt_set = 15;
	}
	/*根据dP/dU的符号进行功率点的位置判断*/
//	if(p_v_deri > ppt->p_v_deri_resolution || p_v_deri < -ppt->p_v_deri_resolution)
//	{/*大于0，工作点在功率点的左侧，增大调控电压*/ /*小于0，工作点在功率点右侧,减小输出电压*/
//		ppt->volt_step = ppt->step_k * p_v_deri;
//		/*volt_step 数据范围限定*/
//		if(ppt->volt_step > ppt->volt_step_max) ppt->volt_step = ppt->volt_step_max;
//		else if(ppt->volt_step < ppt->volt_step_min) ppt->volt_step = ppt->volt_step_min;
//		
//		ppt->volt_set += ppt->volt_step;
//		/*设定点最小值判断*/
//		if(ppt->volt_set < volt_start) ppt->volt_set = volt_start;
//	}
	else
	{/*p_v_deri = 0,认为工作点和功率点重合*/
		ppt->volt_ppt = volt;
		ppt->curr_ppt = curr;
		ppt->power_ppt = ppt->volt_ppt * ppt->curr_ppt;
		if(ppt->volt_set < volt_start) ppt->volt_set = volt_start;
	}
	/**/
	ppt->volt = volt;
	ppt->curr = curr;
	
}



/*******************************************************************************************/

