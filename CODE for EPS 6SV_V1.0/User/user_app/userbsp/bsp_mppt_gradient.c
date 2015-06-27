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
	double p_v_deri; //���ʶԵ�ѹ����
	
	volt_det = volt - ppt->volt;
	curr_det = curr - ppt->curr;
	power_det = curr * volt - ppt->volt * ppt->curr;
//	/*����dP/dU*/
//	p_v_deri = power_det/volt_det;
	p_v_deri = curr + curr_det/volt_det;
	if(p_v_deri > 0.005 || p_v_deri < -0.005)
	{/*����0���������ڹ��ʵ����࣬������ص�ѹ*/ /*С��0���������ڹ��ʵ��Ҳ�,��С�����ѹ*/
		ppt->volt_step = ppt->step_k * p_v_deri;
		/*volt_step ���ݷ�Χ�޶�*/
		if(ppt->volt_step > ppt->volt_step_max) ppt->volt_step = ppt->volt_step_max;
//		else if(ppt->volt_step < ppt->volt_step_min && ppt->volt_step > 0) ppt->volt_step = ppt->volt_step_min;
//		else if(ppt->volt_step > -(ppt->volt_step_min ) && ppt->volt_step < 0) ppt->volt_step = -(ppt->volt_step_min);
		else if(ppt->volt_step < -(ppt->volt_step_max)) ppt->volt_step = -(ppt->volt_step_max);
		
		ppt->volt_set += ppt->volt_step;
		/*�趨����Сֵ�ж�*/
		if(ppt->volt_set < volt_start) ppt->volt_set = volt_start;
		if(ppt->volt_set > 15) ppt->volt_set = 15;
	}
	/*����dP/dU�ķ��Ž��й��ʵ��λ���ж�*/
//	if(p_v_deri > ppt->p_v_deri_resolution || p_v_deri < -ppt->p_v_deri_resolution)
//	{/*����0���������ڹ��ʵ����࣬������ص�ѹ*/ /*С��0���������ڹ��ʵ��Ҳ�,��С�����ѹ*/
//		ppt->volt_step = ppt->step_k * p_v_deri;
//		/*volt_step ���ݷ�Χ�޶�*/
//		if(ppt->volt_step > ppt->volt_step_max) ppt->volt_step = ppt->volt_step_max;
//		else if(ppt->volt_step < ppt->volt_step_min) ppt->volt_step = ppt->volt_step_min;
//		
//		ppt->volt_set += ppt->volt_step;
//		/*�趨����Сֵ�ж�*/
//		if(ppt->volt_set < volt_start) ppt->volt_set = volt_start;
//	}
	else
	{/*p_v_deri = 0,��Ϊ������͹��ʵ��غ�*/
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

