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


#define mppt_num        6  //mppt 转换器数量

#if MPPT_INC_CONDC_EN > 0u
//定义mppt结构体
typedef struct{
	double v_cur,c_cur; //当前电压电流值
	double v_pre,c_pre;//上一次电压电流值
	double v_diff,c_diff;//电压电流差
	double v_diff_min,c_diff_min,r_diff_min;//电压电流最小差值，判断是否为零的比较判据
	double step;        //步进大小
	double in_set;          //设定输出值
	double v_inmax,v_inmin;//输出值最大值，最小值
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

