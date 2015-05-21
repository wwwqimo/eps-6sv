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
**根据P-U曲线 P=U*I 在最大功率点Pmax处，dP/dU = I+U*dI/dU = 0;
**得到         dI/dU = -I/U
**控制流程：
**        1.读取当前电流，电压：c_value,v_value
**        2.求电压，电流差值：v_diff = v_value-v_pre；c_diff=c_value-c_pre
**        3.判断v_diff 是否为0，Y跳转到步骤4，N跳转到步骤5
**        4.(3-Y)->判断c_diff = 0是否成立
**                 Y已达到最大功率点，输出设定不变，当前判断循环结束，跳转步骤6
**                 N跳转步骤4.1
**        4.1(4-N)->判断c_diff>0是否成立，
**                 Y增加扰动输出；跳转步骤6
**                 N减小扰动输出。跳转步骤6
**        5.(3-N)->判断c_diff/v_diff = -c_value/v_value是否成立，
**                 Y满足最大功率点公式，输出设定不变，结束当前循环；跳转步骤6
**                 N跳转步骤5.1
*8        5.1(5-N)->判断c_diff/v_diff >(-c_value/v_value)是否成立，
**                 Y增加扰动输出；跳转步骤6
**                 N减小扰动输出，跳转步骤6
**        6.c_pre = c_value; v_pre = v_value
**          返回步骤1继续执行
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
*mppt 通道初始化程序
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
/*单独mppt通道初始化
*mpptc mppt 结构体
*instep     每次步进大小
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
**mppt 电导增量法实现程序
**para 
**      mpptc    mppt结构体指针
**      volt     采样电压
**      curr     采样电流
*/     
mppt_incond_t mppt_incond(mppt_incond_t * mpptc,double volt,double curr)
{
	mpptc->v_cur = volt;
	mpptc->c_cur = curr;
	//步骤2；求差
	mpptc->c_diff = mpptc->c_cur - mpptc->c_pre;
	mpptc->v_diff = mpptc->v_cur - mpptc->v_pre;
	//步骤3；判断v_diff是否为0
	if((mpptc->v_diff < mpptc->v_diff_min)&&(mpptc->v_diff + mpptc->v_diff > 0))
	{
		//Y 步骤4；判断c_diff是否为0
		//步骤4.1； c_diff > 0.增大输出
		if(mpptc->c_diff > mpptc->c_diff_min)
		{
			mpptc->in += mpptc->step;
		}
		//步骤4.1； c_diff < 0,减小输出
		else if(mpptc->c_diff + mpptc->c_diff_min < 0)
		{
			mpptc->in -= mpptc->step;
		}
	}
	//步骤5；判断c_diff/v_diff = -c_value/v_value是否成立，
	else
	{
		//步骤5.1，判断c_diff/v_diff >(-c_value/v_value)
		//Y 增加扰动输出
		if(mpptc->c_diff/mpptc->v_diff + mpptc->c_cur/mpptc->v_cur > mpptc->r_diff_min)
		{
			mpptc-in += mpptc->step;
		}
		// N减小扰动输出
		else if(mpptc->c_diff/mpptc->v_diff + mpptc->c_cur/mpptc->v_cur < (-mpptc->r_diff_min))
		{
			mpptc->in -=mpptc->step;
		}	
	}
	//步骤6；
	mpptc->c_pre = mpptc->c_cur;
	mpptc->v_pre = mpptc->v_cur;
	//输出范围检查
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

