/*
**************************************************************************************************
**bsp_mppt
**
**
***************************************************************************************************
*/

#include "bsp_mppt.h"

#if MPPT_INC_CONDC_EN > 0u

mppt_incond_t mppt_conv[mppt_num];

static void bsp_mppt_incond_Init(void);
#endif
/*
*******************************************************************************************************
**Incremental conductance method
**����P-U���� P=U*I ������ʵ�Pmax����dP/dU = I+U*dI/dU = 0;
**�õ�         dI/dU = -I/U
**�������̣�
**        1.��ȡ��ǰ��������ѹ��c_value,v_value
**        2.���ѹ��������ֵ��v_diff = v_value-v_pre��c_diff=c_value-c_pre
**        3.�ж�v_diff �Ƿ�Ϊ0��Y��ת������4��N��ת������5
**        4.(3-Y)->�ж�c_diff = 0�Ƿ����
**                 Y�Ѵﵽ����ʵ㣬����趨���䣬��ǰ�ж�ѭ����������ת����6
**                 N��ת����4.1
**        4.1(4-N)->�ж�c_diff>0�Ƿ������
**                 Y�����Ŷ��������ת����6
**                 N��С�Ŷ��������ת����6
**        5.(3-N)->�ж�c_diff/v_diff = -c_value/v_value�Ƿ������
**                 Y��������ʵ㹫ʽ������趨���䣬������ǰѭ������ת����6
**                 N��ת����5.1
*8        5.1(5-N)->�ж�c_diff/v_diff >(-c_value/v_value)�Ƿ������
**                 Y�����Ŷ��������ת����6
**                 N��С�Ŷ��������ת����6
**        6.c_pre = c_value; v_pre = v_value
**          ���ز���1����ִ��
**   
******************************************************************************************************
*/
void bsp_MPPT_Init(void)
{
	#if MPPT_INC_CONDC_EN > 0u
	  bsp_mppt_incond_Init();
	#endif
	
}

#if MPPT_INC_CONDC_EN > 0u

/*
*mppt ͨ����ʼ������
*
*/
static void bsp_mppt_incond_Init(void)
{
	uint8_t i;
	memset(&mppt_conv[0],0,sizeof(mppt_incond_t)*mppt_num);
	for(i=0;i<mppt_num;i++)
	{
		bsp_mppt_incond_Init_indep(&mppt_conv[i],0.1,0.001,0.001,0.001,15.0,4.0);
	}
} 
/*����mpptͨ����ʼ��
*mpptc mppt �ṹ��
*instep     ÿ�β�����С
*
*/
void bsp_mppt_incond_Init_indep(mppt_incond_t * mpptc,double instep,double c_diffmin,double v_diffmin,double r_diffmin,double inmax,double inmin)
{
	mpptc->step = instep;
	mpptc->c_diff_min = c_diffmin;
	mpptc->v_diff_min = v_diffmin;
	mpptc->r_diff_min = r_diffmin;
	mpptc->in_max = inmax;
	mpptc->in_min = inmin;
}
/*
**mppt �絼������ʵ�ֳ���
**para 
**      mpptc    mppt�ṹ��ָ��
**      volt     ������ѹ
**      curr     ��������
*/     
mppt_incond_t mppt_incond(mppt_incond_t * mpptc,double volt,double curr)
{
	mpptc->v_cur = volt;
	mpptc->c_cur = curr;
	//����2�����
	mpptc->c_diff = mpptc->c_cur - mpptc->c_pre;
	mpptc->v_diff = mpptc->v_cur - mpptc->v_pre;
	//����3���ж�v_diff�Ƿ�Ϊ0
	if((mpptc->v_diff < mpptc->v_diff_min)&&(mpptc->v_diff + mpptc->v_diff > 0))
	{
		//Y ����4���ж�c_diff�Ƿ�Ϊ0
		//����4.1�� c_diff > 0.�������
		if(mpptc->c_diff > mpptc->c_diff_min)
		{
			mpptc->in += mpptc->step;
		}
		//����4.1�� c_diff < 0,��С���
		else if(mpptc->c_diff + mpptc->c_diff_min < 0)
		{
			mpptc->in -= mpptc->step;
		}
	}
	//����5���ж�c_diff/v_diff = -c_value/v_value�Ƿ������
	else
	{
		//����5.1���ж�c_diff/v_diff >(-c_value/v_value)
		//Y �����Ŷ����
		if(mpptc->c_diff/mpptc->v_diff + mpptc->c_cur/mpptc->v_cur > mpptc->r_diff_min)
		{
			mpptc-in += mpptc->step;
		}
		// N��С�Ŷ����
		else if(mpptc->c_diff/mpptc->v_diff + mpptc->c_cur/mpptc->v_cur < (-mpptc->r_diff_min))
		{
			mpptc->in -=mpptc->step;
		}	
	}
	//����6��
	mpptc->c_pre = mpptc->c_cur;
	mpptc->v_pre = mpptc->v_cur;
	//�����Χ���
	if(mpptc->in > mpptc->in_max)
	{
		mpptc->in = mpptc->in_max;
	}
	else if(mpptc->in < mpptc->in_min)
	{
		mpptc->in =mpptc->in_min;
	}
	
}
return mpptc;

#endif


void ppt_alloff(void)
{
	
}













/********************************************************************************************************/

