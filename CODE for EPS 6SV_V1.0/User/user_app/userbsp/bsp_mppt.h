/*
**************************************************************************************************
**bsp_mppt
**
**
***************************************************************************************************
*/
#ifndef __BSP_MPPT_H__
#define __BSP_MPPT_H__



#include "user_bsp.h"
#include "user_app.h"
#include "user_cfg.h"


#define mppt_num        6  //mppt ת��������

#if MPPT_INC_CONDC_EN > 0u
//����mppt�ṹ��
typedef struct{
	double v_cur,c_cur; //��ǰ��ѹ����ֵ
	double v_pre,c_pre;//��һ�ε�ѹ����ֵ
	double v_diff,c_diff;//��ѹ������
	double v_diff_min,c_diff_min,r_diff_min;//��ѹ������С��ֵ���ж��Ƿ�Ϊ��ıȽ��о�
	double step;        //������С
	double in_set;          //�趨���ֵ
	double v_inmax,v_inmin;//���ֵ���ֵ����Сֵ
}mppt_incond_t;

#endif


void bsp_MPPT_Init(void);

#if MPPT_INC_CONDC_EN > 0u
void bsp_mppt_incond_Init_indep(mppt_incond_t * mpptc,double instep,double c_diffmin,double v_diffmin,double r_diffmin,double inmax,double inmin);
mppt_incond_t *mppt_incond(mppt_incond_t * mpptc,double volt,double curr);

#endif

void ppt_alloff(void);
#endif /*bsp_mppt.h*/

/****************************************************************************************************/

