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
	double volt;//volt����һ�ε�ѹֵ
	double curr;//curr�ϴε���ֵ
	double volt_step,volt_step_max,volt_step_min;//��ǰ��ѹ�������䷶Χ
	double p_v_deri_resolution;//���ʶԵ�ѹ���� dP/dU
	double step_k; //step��������
	double volt_set;  //�´ε�ѹ�趨ֵ
	double power_ppt,volt_ppt,curr_ppt;  //���ʵ�״̬��
}ppt_gradient_t;


void bsp_ppt_gradient_Init(ppt_gradient_t *ppt,double v_stepmax,double v_stepmin,double p_deri_resolution);
void ppt_gradient(ppt_gradient_t *ppt,double volt, double curr, double volt_start);
#endif

/********************************************************************************************/

