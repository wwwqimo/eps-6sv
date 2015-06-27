/*
*******************************************************************************************************************************
**APP MPPT
**
**
*****************************************************************************************************************************
*/

#include "app_mppt.h"
#include "includes.h"
// the ppt current and voltage values are sampled used the mcu adc,
//extern declaration the adc converted value;
extern __IO uint16_t uhADCConvertedValue[NumofConv];
//extern declaration adc map array
extern uint8_t adc_map[];
//extern declaration adc division
extern float  adc_div[];
extern float  adc_comp[];
//define the eps_ppt_t type struct
extern PPT_Typedef ppt_conv[SV_NUM];

//define 
#define FILTER_DEPTH              20
//define a filter bufer
static uint16_t sf_buffer[FILTER_DEPTH ][16];//定义滑动滤波缓存区

//pid 参数实例化
arm_pid_instance_f32     pid_ppt_para[SV_NUM];
//pid 数据实例化					
PID_Typedef              pid_ppt_var[SV_NUM];

ppt_gradient_t ppt_gradient_data[SV_NUM];
static void ppt_gradient_Init(void);

ppt_pnd_t ppt_pnd_data[SV_NUM];
static void ppt_pnd_Init(void);


ppt_inccond_t ppt_inccond_data[SV_NUM];
static void ppt_inccond_Init(void);

mppt_incond_t ppt_inccond2_data[SV_NUM];
static void ppt_inccond2_Init(void);
#if USER_DEBUG_EN >0u
	#define MPPT_DELAY_TIME       2
#else
	#define MPPT_DELAY_TIME       2
#endif




/**
*@bref 电池恒压充电判断
*@detail
*@para      
*@          bat_volt  电池当前电压值   mv
*@reteval   battery constant charge flag :
*               0= not constant charge; 
*               1= constant charge, set voltage = bat_full_volt+volt_drop;
*               2= constant charge, set voltage = bat_full_volt
*/
uint8_t bat_const_charge(uint16_t bat_volt)
{
	/*电池充电状态标识，当电池超过一定值时进入恒压充电模式，
	当电池电压降到一定值时解除恒压模式
	0=无锁定状态；1=锁定状态*/
	static uint8_t bat_volt_lock=0;//

	if(bat_volt >= bat_full_volt - bat_volt_hyst_up)
	{
		bat_volt_lock = 1;
	}
	else if(bat_volt <= bat_full_volt - bat_volt_hyst_down)
	{
		bat_volt_lock = 0;
	}
	
	if(bat_volt_lock == 0) 
	{
		return 0;
	}
	else /*bat_volt_lock = 1 */
	{
		if(bat_volt < bat_full_volt) return 1;//有锁，若电池未充满，设定为电池电压+volt_drop
		else return 2;//有锁，电池满充，设定输出电压为电池满充电压
	}
}

void app_MPPT(void)
{
	uint8_t i=0,j=0;
	float ppt_set, pwm_out = 0;
	uint8_t const_charge_flag=0;
	uint16_t adc_value[16];
	
	ppt_data_Init(ppt_conv,SV_NUM);
	ppt_pid_Init(pid_ppt_para,pid_ppt_var,SV_NUM);
//	ppt_gradient_Init();
//	ppt_pnd_Init();
//	ppt_inccond_Init();
	ppt_inccond2_Init();
	/*初始通道全部打开*/
	for(i=0;i<SV_NUM;i++)
	{
		PWM_Updata((uint16_t)pwm_out,i);
	}
	/*等待100ms*/
	OSTimeDlyHMSM(0, 0, 0, 100);
	while(1)
	{
		//get the adc value ,fill in the array
		for(i=0;i<16;i++)
		{
			adc_value[i] = pptSlidingFilter(uhADCConvertedValue[i],i);
		}
		//get the converter real value current [mA] voltage [mV]
		pptGetValue(adc_value,ppt_conv);
		
		/*main algorithm loop*/
		for(i=0;i<1/*SV_NUM*/;i++)
		{
			/*恒压充电模式判断*/
			const_charge_flag = bat_const_charge(ppt_conv[i].out_volt);
			/*ppt mode*/
			if(const_charge_flag == 0) 
			{

				if(ppt_conv[i].mode == 0)
				{/*fixed ppt mode*/
					ppt_set = ppt_fixed(&ppt_conv[i],i);
				}
				else
				{
					if(j++>50)
					{
						j=0;
						/*auto ppt mode*/
						ppt_set = ppt_auto(&ppt_conv[i],i);
					}
				}
				
				pid_ppt_var[i].SetValue = ppt_set;
				/*set converter input voltage ,using minus incremental pid*/
				pwm_out = Cal_IncPID_Minus(&pid_ppt_para[i], &pid_ppt_var[i],(float)ppt_conv[i].volt);				
			}
			/*constant charge mode ,constant output voltage = bat_full_volt+bat_volt_drop*/
			else if(const_charge_flag == 1)
			{
				pid_ppt_var[i].SetValue = bat_full_volt/* + bat_volt_drop*/;
				pwm_out = Cal_IncPID_Plus(&pid_ppt_para[i], &pid_ppt_var[i],(float)ppt_conv[i].out_volt);	
			}
			/*constant charge mode ,constant output voltage = bat_full_volt*/			
			else if(const_charge_flag == 2)
			{
				pid_ppt_var[i].SetValue = bat_full_volt;
				pwm_out = Cal_IncPID_Plus(&pid_ppt_para[i], &pid_ppt_var[i],(float)ppt_conv[i].out_volt);	

			}
			else
			{
				return;
			}
#if USER_DEBUG_EN > 0u
			if((ppt_conv[i].out_volt >= (bat_full_volt - bat_volt_hyst_up)))
			{
//				printf("bat_volt =      %4d\r\n",ppt_conv[i].out_volt);
//			  printf("pwm_out[%1d] =  %4d\r\n",i,(uint16_t)pwm_out);
//			  printf("const charge flag = %d\r\n",const_charge_flag);
			}
#endif
		  PWM_Updata((uint16_t)pwm_out,i);
		}

		OSTimeDlyHMSM(0, 0, 0, MPPT_DELAY_TIME);
	}

}
	/*获取当前通道对应电池板温度*/
static int16_t get_svtemp(uint16_t chan)
{
	return 25;
}
static float ppt_fixed(PPT_Typedef *ppt,uint16_t chan)
{
	int16_t temp=25;//定义温度变量
	/*获取当前通道对应电池板温度*/

	float value_set = (float)ppt->ppt_volt;
	temp = get_svtemp(chan);
	value_set += (temp-25)*SV_NUM*13; 
	return value_set;
}

/**
*@bref     ppt auto algorithim function
*@detail   
*@para
*@reteval  
*/
static float ppt_auto(PPT_Typedef *ppt,uint16_t chan)
{
	float value_set;
	
//	ppt_inccond(&ppt_inccond_data[chan],(double)(ppt->volt)/1000,(double)(ppt->curr)/1000,8.4);
//	value_set = (ppt_inccond_data[chan].volt_set)*1000;	
	
//	ppt_pnd(&ppt_pnd_data[chan],(double)(ppt->volt)/1000,(double)(ppt->curr)/1000, 8.4);
//	value_set = (ppt_pnd_data[chan].volt_set)*1000;
	
	mppt_incond(&ppt_inccond2_data[chan],(double)(ppt->volt)/1000,(double)(ppt->curr)/1000);
	value_set = (ppt_inccond2_data[chan].in_set)*1000;
//	printf("mppt-set voltage[%d] = %5f\r\n",chan,value_set);
	return value_set;
}
static void ppt_inccond_Init(void)
{
	uint8_t i;
	memset(ppt_inccond_data,0,SV_NUM*sizeof(ppt_inccond_t));
	for(i=0;i<SV_NUM;i++)
	{
//		void bsp_ppt_inccond_Init(ppt_inccond_t *ppt,double v_setmax,double v_setmin,double v_stepmax,double k,double p_v_deri_res)
		bsp_ppt_inccond_Init(&ppt_inccond_data[i],15,0,0.5,0.2,0.01);
	}
}
static void ppt_inccond2_Init(void)
{
	uint8_t i;
	memset(ppt_inccond2_data,0,SV_NUM*sizeof(mppt_incond_t));
	for(i=0;i<SV_NUM;i++)
	{
		bsp_mppt_incond_Init_indep(&ppt_inccond2_data[i],0.01,0.01,0.003,0.01,15,8.4);

	}
}
static void ppt_pnd_Init(void)
{
	uint8_t i;
	memset(ppt_pnd_data,0,SV_NUM*sizeof(ppt_pnd_t));
	for(i=0;i<SV_NUM;i++)
	{
		bsp_ppt_pnd_Init(&ppt_pnd_data[i],15, 0, 0.1,0.001);
	}

}
static void ppt_gradient_Init(void)
{
	uint8_t i;
	memset(ppt_gradient_data,0,SV_NUM*sizeof(ppt_gradient_t));
	for (i=0;i<SV_NUM;i++)
	{
		bsp_ppt_gradient_Init(&ppt_gradient_data[i],0.5,0.01,0.001);
	}
}
//ppt struct init 
//the function is called before the ppt function running
//para mode :the ppt mode pointer 
//para ppt_volt : if the ppt converter worked in the fixed mode ,the value will be the ppt pointer value
void ppt_data_Init(PPT_Typedef *ppt,uint8_t ppt_num)
{
	uint8_t i;
	for (i=0;i<ppt_num;i++)
	{
//		memset((ppt+i),0,sizeof(PPT_Typedef));
		(ppt+i)->ppt_volt = 11750;
		(ppt+i)->mode = 1;
	}	
}
/**
*@bref ppt pid paras init
*@detail
*@para
*@reteval
*/
void ppt_pid_Init(arm_pid_instance_f32 *pid_para, PID_Typedef *pid_var,uint8_t pid_num)
{
	uint8_t i;
	for(i=0;i<pid_num;i++)
	{
		memset((pid_para+i),0,sizeof(arm_pid_instance_f32));
		(pid_para+i)->Kp = 0.005;
		(pid_para+i)->Ki = 0;
		(pid_para+i)->Kd = 0;
		memset((pid_var+i),0,sizeof(PID_Typedef));

		(pid_var+i)->OutMax = 1000;
		(pid_var+i)->OutMin = 0;
		(pid_var+i)->Output = (pid_var+i)->OutMax;
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
	return (uint16_t)(sum/FILTER_DEPTH);
}

/**********************************************************************************************************************/
/*
**将uint16_t格式的adc值转换为真实值
** 电压量单位 mv
** 电流量单位 ma
** 温度量单位 degc
*/
static void pptGetValue(uint16_t *adc_uint,PPT_Typedef *adc_dest)
{
	uint8_t i;

	for(i=0;i<PPT_NUM;i++)
	{
		(adc_dest+i)->curr = (uint16_t)(((float)(*(adc_uint+adc_map[i])))*(adc_div[i])*(1+adc_comp[i])*ADC_REF/ADC_FullScale);
		(adc_dest+i)->volt = (uint16_t)(((float)(*(adc_uint+adc_map[i+PPT_NUM])))*(adc_div[i+PPT_NUM])*(1+adc_comp[i+PPT_NUM])*ADC_REF/ADC_FullScale);
		(adc_dest+i)->out_volt = (uint16_t)((float)(*(adc_uint+adc_map[14]))*(adc_div[14])*(1+adc_comp[14])*ADC_REF/ADC_FullScale);
	}
}











/**************************************************************************************************************************/

