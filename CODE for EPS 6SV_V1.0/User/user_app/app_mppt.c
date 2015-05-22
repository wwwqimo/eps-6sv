/*
*******************************************************************************************************************************
**APP MPPT
**
**
*****************************************************************************************************************************
*/

#include "app_mppt.h"

// the ppt current and voltage values are sampled used the mcu adc,
//extern declaration the adc converted value;
extern __IO uint16_t uhADCConvertedValue[NumofConv];
//extern declaration adc map array
extern uint8_t adc_map[];
//extern declaration adc division
extern float  adc_div[];
#define PPT_NUM                 6 //ppt converter number
//define the eps_ppt_t type struct
eps_ppt_t ppt_conv[PPT_NUM];
//define 
#define FILTER_DEPTH              10
//define a filter bufer
static uint16_t sf_buffer[FILTER_DEPTH ][PPT_NUM];//定义滑动滤波缓存区




#if USER_DEBUG_EN >0u
	#define MPPT_DELAY_TIME       500
#else
	#define MPPT_DELAY_TIME       2
#endif



#define MPPT_V_Div    6.0f
#define MPPT_C_Multi  1.25f
extern float    Value_Real[NumofConv];//换算后真是采样值


//仿真数据
#if MPPT_SIM_EN >0u
//I = -V/20 +0.5;
float V_MPPT_SIM[mppt_num];
float I_MPPT_SIM[mppt_num];
#endif

#if MPPT_INC_CONDC_EN > 0u

extern MPPT_Typedef MPPT_CONV[mppt_num];

#endif

void app_MPPT(void)
{
	
	uint8_t i;
	uint16_t mppt_dacout;
	while(1)
	{
		
		
		OSTimeDlyHMSM(0, 0, 0, MPPT_DELAY_TIME);
	}

}

//仿真函数
#if MPPT_INC_CONDC_SIM_EN > 0u
void MPPT_SIM(void)
{
	uint8_t i;

		for (i=0;i<mppt_num;i++)
		{
			V_MPPT_SIM[i] = MPPT_CONV[i].out;
			I_MPPT_SIM[i] = -V_MPPT_SIM[i]/20 + 0.5f;
		}
		//MPPT_INC_CONDC SIMULATION

		for (i=0;i<mppt_num;i++)
		{
			MPPT_CONV[i].v_cur = V_MPPT_SIM[i];
			MPPT_CONV[i].c_cur = I_MPPT_SIM[i];
		}	
}
#endif

//ppt struct init 
//the function is called before the ppt function running
//para mode :the ppt mode pointer 
//para ppt_volt : if the ppt converter worked in the fixed mode ,the value will be the ppt pointer value
void ppt_data_Init(uint8_t *mode,uint16_t *ppt_volt)
{
	uint8_t i;
	for (i=0;i<PPT_NUM;i++)
	{
		memset(&ppt_conv[0],0,sizeof(eps_ppt_t));
		ppt_conv[i].ppt_volt = 11750;
	}	
}

/*
**********************************************************************************************************************
** 滑动滤波算法
** * adc_value 输入数据指针  
** adc_size输入数据长度
**
******************************************************************************************************************
*/
static uint16_t  pptSlidingFilter(uint16_t adc_value,uint16_t adc_count)
{
	static uint16_t ptr[16] = {0};
	uint16_t k;
	uint32_t sum =0;

	sf_buffer[ptr[adc_count]++][adc_count] = adc_value;
	
	if(ptr[adc_count] >=FILTER_DEPTH)	ptr[adc_count] = 0;
	
	for(k=0;k<FILTER_DEPTH;k++)
	{
		sum += sf_buffer[k][adc_count];
	}
	return (uint16_t)sum/FILTER_DEPTH;
}

/**********************************************************************************************************************/
/*
**将uint16_t格式的adc值转换为真实值
** 电压量单位 mv
** 电流量单位 ma
** 温度量单位 degc
*/

static void pptGetValue(uint16_t *adc_uint,eps_ppt_t *adc_dest)
{
	uint8_t i;
	uint16_t * adc_dest1;
	adc_dest1 = (uint16_t*)adc_dest;

	for(i=0;i<PPT_NUM;i++)
	{
		(adc_dest+i)->curr = (uint16_t)(((float)(*(adc_uint+adc_map[i])))*adc_div[i]*ADC_REF/ADC_FullScale);
		(adc_dest+i)->volt = (uint16_t)(((float)(*(adc_uint+adc_map[i+PPT_NUM])))*adc_div[i+PPT_NUM]*ADC_REF/ADC_FullScale);
	}

}











/**************************************************************************************************************************/

