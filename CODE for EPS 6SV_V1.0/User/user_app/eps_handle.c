
#include "user_app.h"
#include "user_cfg.h"
#include <math.h>
#include <stdio.h>
char * bat_status_char[]={"bat full charged","bat normal voltage","bat enter safe mode",
													"bat enter critical mode","bat hard off"};	
	
#define batSeries_num     2
#define batParallel       2														
#define batC              (2600*batParallel)
#define batResolution     12
#define batCurve_num      3
//battery capacity to voltage map array											
uint16_t batSOCcurve[batCurve_num + 1][batResolution]={
													4200,4060,3980,3920,3870,3820,3790,3770,3740,3680,3450,3000,  /*0.2c 开路电压下的电池容量*/
													4200,3970,3870,3790,3730,3680,3650,3620,3580,3510,3420,3000,   /*0.5c下电池容量*/
													4200,3800,3750,3650,3625,3580,3550,3520,3480,3400,3300,0000,   /*1c */
													100, 90,  80,  70,  60,  50,  40,  30,  20,  10,  5,   0};     /*电量百分比*/

//regulated and unregulated outputs current limitation array
uint16_t c_out_limit[REG_NUM+UREG_NUM]={1000,1000,1000,1000,1000,1000,1000,1000,200};

/**
*@bref output channel switch
*@para    chan     ;NewState
*@reteval none
*/
static void out_channel(uint8_t chan,FunctionalState NewState)
{
	out_fault_en(DISABLE);
	switch (chan)
	{
		case OUT_GPSA:
			OUTPUT_REG1(NewState);
		break;
		case OUT_GPSB:
			OUTPUT_REG2(NewState);
		break;
		case OUT_MTQ:
			OUTPUT_UREG1(NewState);
		break;
		case OUT_WHEELA:
			OUTPUT_UREG2(NewState);
		break;
		case OUT_WHEELB:
			OUTPUT_UREG3(NewState);
		break;
		case OUT_USB:
			OUTPUT_UREG4(NewState);
		break;
		case OUT_RES:
			OUTPUT_REG3(NewState);
		break;
		case OUT_FIPEX5V:
			OUTPUT_REG4(NewState);
		break;
		case OUT_FIPEX3V:
			OUTPUT_REG5(NewState);
		break;
		case OUT_HEAT:
			OUTPUT_HS(NewState);
			break;
		case OUT_DEPLOY:
			OUTPUT_DS(NewState);
		break;
		case OUT_POWERS:
			OUTPUT_PS(NewState);
		break;
		case OUT_PSW:
			OUTPUT_PSW(NewState);
		break;
		default:
			#if USER_DEBUG_EN >0u
			printf("OUTPUT wrong channel!!");
			#endif
			break;
	}
	out_fault_en(ENABLE);
}
/**
*@bref     清楚选定通道的状态，设为初始值 off
*@detail   本函数只是清楚状态变量，不改变端口状态
*@detail   若要强制改变状态，请使用out_state_setforce()函数
*@para     channel  :selected channel
*@reteval  none*/
void out_state_clear(uint8_t chan)
{
	if(chan < UREG_NUM+REG_NUM)
	{
		eps_state.out_state[chan] = OUTPUT_OFF;
	}
	else
	{
		printf("ERROR EPS: The selected channel is out of range!");
	}
}
/**
*@bref     强制设置选定通道为开或关
*@detail
*@para     chan ：selected channel ；NewState：通道新状态 
*@reteval  none
*/
void out_state_setforce(uint8_t chan,FunctionalState NewState)
{
	if(chan < UREG_NUM+REG_NUM)
	{
		out_channel(chan, NewState);
		if(NewState == ENABLE)
		{
			eps_state.out_state[chan] = OUTPUT_ON;
		}
		else
		{
			eps_state.out_state[chan] = OUTPUT_OFF;
		}
	}
	else
	{
		printf("ERROR EPS: The selected channel is out of range!");
	}
}
/**
*@bref 电源板输出控制
*@detail
*@para   chan  对应通道 OUT_GPSA,OUT_GPSB,OUT_MTQ,OUT_WHEELA,
									      OUT_WHEELB,OUT_USB,OUT_FIPEX5V,OUT_FIPEX3V
									      OUT_RES,OUT_HEAT,OUT_DEPLOY
         NewState 通道状态  ENABLE,UNABLE
*@reteval  通道状态  若打开返回OUTPUT_ON =1;
*                    若关闭返回OUTPUT_OFF =0;
*                    若对应通道错误，返回错误标号>1
*                    OUTPUT_ERR =2 OUTPUT_OFFERR=3,OUTPUT_SFTRY=4,
*										 OUTPUT_HDTRY=5,OUTPUT_TRYOK=6,OUTPUT_HDERR =7
*                    OUTPUT_OFFERR不处理，调用程序作后续处理
*                    OUTPUT_HDERR硬件错误，调用程序自行后续处理
*                    其他为中间状态，最后会进入0,1,3,7状态中的一个
*/
output_state_t out_en(uint8_t chan,FunctionalState NewState)
{
	if(chan == OUT_DEPLOY)
	{
		out_channel(OUT_DEPLOY,NewState);
		if(NewState == ENABLE)return OUTPUT_ON;
		else	return OUTPUT_OFF;
	}
	if(chan == OUT_HEAT)
	{
		out_channel(OUT_HEAT,NewState);
		if(NewState == ENABLE)return OUTPUT_ON;
		else	return OUTPUT_OFF;
	}
	else
	{
		if(eps_state.out_state[chan] == OUTPUT_OFF ||
				eps_state.out_state[chan] == OUTPUT_ON)
		{
			out_channel(chan,NewState);
			if(NewState == ENABLE) 
			{
		  	eps_state.out_state[chan] = OUTPUT_ON;
				return OUTPUT_ON;
			}
			else /*(NewState == DISABLE) */
			{
				eps_state.out_state[chan] = OUTPUT_OFF;
				return OUTPUT_OFF;
			}
		}
		else return eps_state.out_state[chan];	
	}		
}
/**
*@bref  all out channels switch (GPSA,GPSB,WHEELA,WHEELB,MTQ,USB)
*@para  NewState ENABLE;DISABLE
*@reteval if NewState=DISABLE,return -1;
*         if NewState=ENABLE,return -1;
*         if there is one or more channel cannot open or shutdown
*         then return the last error channel num
*/
int8_t outall_en(FunctionalState NewState)
{
	output_state_t state = OUTPUT_OFF;
	if(NewState == ENABLE)
	{
		state = out_en(OUT_GPSA,ENABLE);
		if(state != OUTPUT_ON) return OUT_GPSA;
		state = out_en(OUT_GPSB,ENABLE);
		if(state != OUTPUT_ON) return OUT_GPSB;
		state = out_en(OUT_WHEELA,ENABLE);
		if(state != OUTPUT_ON) return OUT_WHEELA;
		state = out_en(OUT_WHEELB,ENABLE);
		if(state != OUTPUT_ON) return OUT_WHEELB;
		state = out_en(OUT_MTQ,ENABLE);
		if(state != OUTPUT_ON) return OUT_MTQ;
		state = out_en(OUT_USB,ENABLE);
		if(state != OUTPUT_ON) return OUT_USB;
		return -1;
	}
	else
	{
		state = out_en(OUT_GPSA,DISABLE);
		if(state != OUTPUT_OFF) return OUT_GPSA;
		state = out_en(OUT_GPSB,DISABLE);
		if(state != OUTPUT_OFF) return OUT_GPSB;
		state = out_en(OUT_WHEELA,DISABLE);
		if(state != OUTPUT_OFF) return OUT_WHEELA;
		state = out_en(OUT_WHEELB,DISABLE);
		if(state != OUTPUT_OFF) return OUT_WHEELB;
		state = out_en(OUT_MTQ,DISABLE);
		if(state != OUTPUT_OFF) return OUT_MTQ;
		state = out_en(OUT_USB,DISABLE);
		if(state != OUTPUT_OFF) return OUT_USB;
		return -1;		
	}
}

void eps_enter_normal(void)
{
	if(eps_state.out_state[OUT_GPSA] == OUTPUT_ON)
		out_en(OUT_GPSA,ENABLE);
	if(eps_state.out_state[OUT_GPSB] == OUTPUT_ON)
		out_en(OUT_GPSB,ENABLE);
	if(eps_state.out_state[OUT_MTQ] == OUTPUT_ON)
		out_en(OUT_MTQ,ENABLE);
	if(eps_state.out_state[OUT_WHEELA] == OUTPUT_ON)
		out_en(OUT_WHEELA,ENABLE);
	if(eps_state.out_state[OUT_WHEELB] == OUTPUT_ON)
		out_en(OUT_WHEELB,ENABLE);
	if(eps_state.out_state[OUT_USB] == OUTPUT_ON)
		out_en(OUT_USB,ENABLE);
	if(eps_state.out_state[OUT_FIPEX5V] == OUTPUT_ON)
		out_en(OUT_FIPEX5V,ENABLE);
	if(eps_state.out_state[OUT_FIPEX3V] == OUTPUT_ON)
		out_en(OUT_FIPEX3V,ENABLE);	
}
void eps_enter_safe(void)
{
	output_state_t state;
	state = eps_state.out_state[OUT_MTQ];
	out_en(OUT_MTQ,DISABLE);
	if(state != OUTPUT_ON)
	eps_state.out_state[OUT_MTQ] = state;
}
void eps_enter_critical(void)
{
	int8_t state;
	state = outall_en(DISABLE);
	if(state != -1)
	{
		out_en(OUT_GPSA,DISABLE);
		out_en(OUT_GPSB,DISABLE);
		out_en(OUT_WHEELA,DISABLE);
		out_en(OUT_WHEELB,DISABLE);
		out_en(OUT_USB,DISABLE);
		out_en(OUT_MTQ,DISABLE);
	}
	out_en(OUT_HEAT,DISABLE);
}
void eps_allin_off(void)
{
	
}
void eps_allin_on(void)
{	
	
}
/**
*@bref battery heate switch
*@para none
*@reteval   state 
*           no error  return OUTPUT_ON
*           error     return error mesg
*/
uint8_t bat_heater_on(void)
{
	return out_en(OUT_HEAT,ENABLE);
}
/**
*@bref    battery heat switch off
*@para    none
*@reteval state
*              no error  return OUTPUT_OFF
*              error     return error mesg
*/
uint8_t bat_heater_off(void)
{
	return 	out_en(OUT_HEAT,DISABLE);
}

void eps_data_Init(void)
{
	//init eps_adc
	memset(&eps_adc_data,0,sizeof(eps_hk_adc_t));
	//init eps_state structure 
	memset(&eps_state,0,sizeof(eps_hk_state_t));
	eps_state.conv_state[0] = CONV_ON;
	eps_state.conv_state[1] = CONV_ON;
	//init eps_bat structure
	memset(&eps_bat,0,sizeof(eps_bat_t));
	eps_bat.bat_heater_mode = 0;
	eps_bat.bat_fullcap = 38480000;
	eps_bat.bat_percent = 100;
	eps_bat.bat_total_percent = 100;
	eps_bat.bat_state = BAT_NORMAL;
	eps_bat.bat_status =  bat_status_char[BAT_NORMAL];
}
void eps_data_handling()
{
	//
	bat_data_processing(&eps_adc_data,&eps_bat);
	//
	output_data_processing(&eps_adc_data,&eps_state);
	//
	conv_data_processing(&eps_adc_data,&eps_state);
	//
	temp_data_processing(&eps_adc_data,&eps_state);
}
/**
*@bref    eps tempture manager funtion
*@detail
*@para    eps_adc   
*         eps_state
*@reteval none
*/
static void temp_data_processing(eps_hk_adc_t * eps_adc, eps_hk_state_t * eps_state)
{
	
}
/**
*@bref converter state 
*@para volt : converter voltage
*      ....
*@reteval    state
*/
static conv_state_t conv_state(uint16_t volt,uint16_t max,uint16_t min,uint16_t errmax,uint16_t errmin)
{
	if(volt > errmax || volt < errmin) return CONV_ERR;
	else if(volt > max) return CONV_OV;
	else if(volt < min) return CONV_UV;
	else return CONV_ON;
}
/**
*@bref 
*@detail
//CONVERTER state enum
typedef enum{
	CONV_OFF,
	CONV_ON,
	CONV_ERR,
	CONV_UV,           //converter output voltage too low
	CONV_OV,           //converter output voltage too high
	CONV_OC,           //converter output current too large
}conv_state_t;
*@para
*@reteval
*/

static void conv_data_processing(eps_hk_adc_t *eps_adc,eps_hk_state_t *eps_state)
{
	uint16_t conv1_max = 5250;
	uint16_t conv1_min = 4750;
	uint16_t conv1_errmax = 6000;
	uint16_t conv1_errmin = 4000;
	uint16_t conv2_max = 3450;
	uint16_t conv2_min = 3150;
	uint16_t conv2_errmax = 4000;
	uint16_t conv2_errmin = 2000;
	uint16_t conv_volt = 0;
	conv_state_t conv_state_temp = CONV_ON;
	conv_volt = eps_adc->v_5v;
	conv_state_temp = conv_state(conv_volt,conv1_max,conv1_min,conv1_errmax,conv1_errmin);
	eps_state->conv_state[1] = conv_state_temp;
	conv_volt = eps_adc->v_3v;
	conv_state_temp = conv_state(conv_volt,conv2_max,conv2_min,conv2_errmax,conv2_errmin);
	eps_state->conv_state[0] = conv_state_temp;
}

/**
*@bref    output protection function
*@para
*@reteval
typedef enum{
	OUTPUT_OFF = 0, //output off     if c_out[i]>c_off_offset enter OUTPUT_ERR
	OUTPUT_ON,      //output on      if c_out[i]>c_out_limit[i],enter OUTPUT_ERR
	OUTPUT_ERR,     //output err     OUTPUT_TRY 和 OUTPUT_HDTRY 只有在OUT_ERR之后才会出现
	OUTPUT_OFFERR,  //当output 处于off状态时，由于不可知因素导致通道处于开通状态
	OUTPUT_SFTRY,   //mannual soft autotry
	OUTPUT_HDTRY,   //hardware autotry
	OUTPUT_TRYOK,   //output aytotry ok,之后进入OUTPUT_ON 或 OUTPUT_OFF状态
	OUTPUT_HDERR,   //output hardware err,this channel output will not be opened
}output_state_t;
*/
static void output_data_processing(eps_hk_adc_t *eps_adc,eps_hk_state_t *eps_state)
{
	uint8_t i;
	// set turn on time and turn off time
	// if the eps_state.out_Ton > ton ,then the function is not active	
	// if the eps_state.out_Toff > toff ,then the function is not active	
	uint16_t ton = 10;  //ticks   1S
	uint16_t toff = 10; //ticks   1S
	uint16_t tout_hderr = 200; //ticks    20S
	uint16_t c_off_offset=10;//mA
	static output_state_t pre_state[REG_NUM + UREG_NUM] = {OUTPUT_OFF};//define output pre state enum variable
	
	ton = (1000/USER_TIME_DLY);
	toff = (1000/USER_TIME_DLY);
	tout_hderr = (20000/USER_TIME_DLY);
	
	for(i=0;i<(REG_NUM + UREG_NUM);i++)
	{
		switch (eps_state->out_state[i])
		{
			case OUTPUT_OFF:
				if(eps_adc->c_out[i] > c_off_offset)
				{
					out_channel(i,DISABLE);
					eps_state->out_state[i] = OUTPUT_OFFERR;
					pre_state[i] = OUTPUT_OFF;
				}
				else
				{
					break;
				}
			case OUTPUT_OFFERR:
				eps_state->out_faults++;
				eps_state->out_fault[i]++;
				break;
			case OUTPUT_ON:
				if(eps_adc->c_out[i] > c_out_limit[i])
				{
					eps_state->out_state[i] = OUTPUT_ERR;
					eps_state->out_ocp[i]++;
					//用于在OUTPUT_ERR中的判断
					pre_state[i] = OUTPUT_ON;//
				}
				else
				{
					pre_state[i] = OUTPUT_OFF;//
					break;
				}
//				break;
			case OUTPUT_ERR:
				out_channel(i,DISABLE);
				//表明是由于软件设置电流限制导致的error
				if(pre_state[i] == OUTPUT_ON)//the error was caused by over current case
				{
					eps_state->out_state[i] = OUTPUT_SFTRY;
					eps_state->out_Ton[i] = ton; 
					pre_state[i] = OUTPUT_OFF; //SET as default value
				}
				else /*if(pre_state[i] == OUTPUT_ERR)*/ //表明是由于硬件fault中断导致的error
				{
					eps_state->out_state[i] = OUTPUT_HDTRY;
				}
				
				eps_state->out_fault[i]++; //单个通道对应的错误加1
				eps_state->out_faults++;//所有的错误量加1

				//注释掉break；程序继续往下走
//				break;
				//软件设定等待一段时间开启故障通道，设定时间在上一个case
			case OUTPUT_SFTRY:
				if((eps_state->out_Ton[i] > 0) && (eps_state->out_Ton[i] < ton+1)) 
				{
					eps_state->out_Ton[i]--;
					eps_state->out_state[i] = OUTPUT_SFTRY;
					//设定时间到，打开故障通道
					if(eps_state->out_Ton[i] == 0)
					{
						out_channel(i,ENABLE);
						
						//设定故障通道关断时间，
						eps_state->out_Toff[i] = toff;
					}

				}
				//判断是否在Toff时间内
				//在此时间内没有出现电流超出限制事件，则保持通道开通
				//否则关闭通道 进入OUTPUT_HDERR 模式
				//此模式可能是输出通道硬件故障也可能是负载故障
				if((eps_state->out_Toff[i] > 0) && (eps_state->out_Toff[i] < toff+1)) 
				{
					eps_state->out_Toff[i]--;
					if(eps_adc->c_out[i] > c_out_limit[i] && eps_state->out_Toff[i] < toff/2)
					{
						out_channel(i,DISABLE);
						eps_state->out_Toff[i] = 0;
						eps_state->out_state[i] = OUTPUT_HDERR;
						eps_state->out_Thderr[i] = tout_hderr;
					}
					else
					{
						if(eps_state->out_Toff[i] == 0) 
						{
							eps_state->out_Toff[i] = 0;
							eps_state->out_state[i] = OUTPUT_TRYOK;

						}
					}
				}
				break;
			case OUTPUT_HDTRY:
				//首次进入HRTRY模式，进行设定
				if(pre_state[i] != OUTPUT_ERR)
				{ 
					//打开故障通道，设定一定时间
					//在规定时间内跳出故障模式则表明通道正常
					out_channel(i,ENABLE);
					//通道状态设为打开，在1S内若再次发生，则中断故障比软件限流故障要快，会再次进入该模式
					eps_state->out_state[i] = OUTPUT_TRYOK;
					eps_state->out_Toff[i] = toff;
					pre_state[i] = OUTPUT_ERR;
//					break;
				}
				//后面再次进入
				else
				{ //在规定时间内没能跳出，则关断当前通道
					if(eps_state->out_Toff[i]-- == 0)
					{
						out_channel(i,DISABLE);
						//设定钙通道HARD ERROR
						eps_state->out_state[i] = OUTPUT_HDERR;
						eps_state->out_Thderr[i] = tout_hderr;
					}
				}
				break;
			case OUTPUT_TRYOK:
				out_channel(i,ENABLE);
				eps_state->out_state[i] = OUTPUT_ON;
				pre_state[i] = OUTPUT_OFF;
				break;
			case OUTPUT_HDERR:
				//lock the output in disable state
				out_channel(i,DISABLE);
				pre_state[i] = OUTPUT_OFF;
				//lock the output state in HARDWARE ERROE STATE,
				eps_state->out_state[i] = OUTPUT_HDERR;
				if(eps_state->out_Thderr[i]-- > 0)
				{
					if(eps_state->out_Thderr[i] == 0) 
					{
						out_channel(i,ENABLE);
						eps_state->out_state[i] = OUTPUT_ON;
					}
				}
				break;
			default:
			break;
		}
	}

}

/**
*@bref battery permanent capacity percent calculation function
*@para
*@eteval
*/
static uint16_t batSOCpermanent(eps_bat_t *bat,uint16_t volt,uint16_t curr)
{
	return 0;
}
/**
*@bref      battery remaining capacity function
*@bref      the function is realized in  voltage method
*@para     :
**    bat  :eps_bat struct ,it will be filled with the result
**		volt :battery voltage
**    curr :battery current
*@reteval  : the calculated result of the battery remaining capacity
*/
static uint16_t batSOC(eps_bat_t *bat,uint16_t volt,uint16_t curr)
{
	uint8_t i;
	uint8_t curve_select; 
	uint16_t soc;
	uint16_t volt_step,volt_diff,soc_diff;
	if(curr < batC*0.2)
	{
		curve_select =0;
	}
	else if(curr < batC*0.5)
	{
		curve_select = 1;
	}
	else  //curr > 0.5*batC 
	{
		curve_select = 2;
	}
	
	for(i=0;i<batResolution;i++)
	{
		if(volt > batSeries_num * batSOCcurve[curve_select][i])
		{
			if(i == 0)//full of charged
			{
				soc = 100;
				break;
			}

			volt_diff = volt/batSeries_num - batSOCcurve[curve_select][i];

			volt_step = batSOCcurve[curve_select][i-1] - batSOCcurve[curve_select][i];

			soc_diff = batSOCcurve[batCurve_num][i-1] - batSOCcurve[batCurve_num][i];

			soc = soc_diff * volt_diff/volt_step + batSOCcurve[batCurve_num][i];
			break;
		}
	}
	bat->bat_percent = soc;
	return soc;
}
/**
*@bref 返回当前电压，外部充电接通之前和接通之后的压差
*@detail
*@para   
*@reteval
*/
static uint16_t bat_charge_voltdrop(uint16_t bat_volt)
{
	return 10;//mv
}
/*
typedef enum{
	BAT_DISCH = 0,
	BAT_CHARGE,
	BAT_NOCURR
}bat_charge_state_t;
*/
/**
*@bref    电池当前充放电状态
*@detail
*@para
*@reteval bat_curr ：电池电流，正表示充电电流，负表示放电电流
*/
static uint16_t bat_charge(eps_hk_adc_t *eps_adc, eps_bat_t *eps_bat )
{
	uint8_t i;
	uint8_t charge_source = 0;//0=SV source;1=External source
	uint16_t curr_hyst = 2;//mA
	uint16_t volt_svin_hyst = 200;//mV
	int16_t  bat_curr;     //????,???????,???????
	uint16_t bat_curr_in,bat_curr_out;
	
//	bat_charge_state_t bat_charge_temp = BAT_NOCURR;
	/*????????,????????????*/
	
	uint16_t v_bus_cur;
	
	static bat_charge_state_t bat_charge_state_back;
	static uint16_t v_bus_old, bat_charge_cnt, v_bus_back;
	static uint16_t bat_voltsqe[20] = {0};
	static uint8_t bat_volt_ptr = 0;//
	static uint8_t bat_volt_flag = 0; 
	/*?????????????
	???SV????6?,
	?????????,????????
	????????????????*/
	/*6?SV??*/
	for(i=0;i<SV_NUM;i++)
	{
		if(eps_adc->v_sv[i] > eps_adc->v_bus + volt_svin_hyst)
		{
			/*???????????????????SV source
			????*/
			charge_source = 0;
			break;
		}
		else
		{
			charge_source = 1;
		}
	}
	//??????????????????
	eps_bat->bat_charge = BAT_DISCH;
	bat_curr_out = eps_adc->c_bus;
	/*SV source*/
	if(charge_source == 0)
	{
		bat_curr_in = eps_adc->c_sv[0]+eps_adc->c_sv[1]+eps_adc->c_sv[2]
								+eps_adc->c_sv[3]+eps_adc->c_sv[4]+eps_adc->c_sv[5];
		
		if(bat_curr_in > bat_curr_out + curr_hyst)
		{
			eps_bat->bat_charge = BAT_SV_CHARGE;
			bat_curr = bat_curr_in - bat_curr_out;
		}
		else if(bat_curr_in < bat_curr_out - curr_hyst)
		{
			eps_bat->bat_charge = BAT_DISCH;
			bat_curr = bat_curr_in - bat_curr_out;
		}
		else
		{
			eps_bat->bat_charge = BAT_NOCURR;
			bat_curr = 0;
		}
		return bat_curr;
	}
	else/*??????*/  //modify
	{
		
		v_bus_cur = eps_adc->v_bus;
		switch(eps_bat->bat_charge)
		{
			case BAT_DISCH:  //??????
				if(v_bus_cur > v_bus_old)  //????????
				{
					if(v_bus_cur - v_bus_old > bat_charge_voltdrop(v_bus_cur))  //????????????
					{
						eps_bat->bat_charge = BAT_EXT_CHARGE_PRE;
						bat_charge_cnt = 2000;
						v_bus_back = v_bus_old;
						bat_charge_state_back = BAT_DISCH;
					}
				}
			break;
			case BAT_EXT_CHARGE_PRE:  //????????
				if(bat_charge_cnt > 0)
				{
					if(v_bus_cur - v_bus_back > bat_charge_voltdrop(v_bus_back))
					{
						bat_charge_cnt--;
					}
					else
					{
						bat_charge_cnt = 0;
						eps_bat->bat_charge = bat_charge_state_back;
					}
				}
				else
				{
					bat_charge_cnt = 0;
					eps_bat->bat_charge = BAT_EXT_CHARGE;
				}	
			break;
				
			case BAT_EXT_CHARGE:
				if(v_bus_cur < v_bus_old)
				{
					eps_bat->bat_charge = BAT_DISCH_PRE;
					bat_charge_cnt = 500;
					v_bus_back = v_bus_old;
					
					bat_charge_state_back = BAT_EXT_CHARGE;
					
				}
			break;
				
			case BAT_DISCH_PRE:	
				if(bat_charge_cnt > 0)
				{
					if(v_bus_cur < v_bus_back)
					{
						bat_charge_cnt--;
					}
					else
					{
						bat_charge_cnt = 0;
						eps_bat->bat_charge = bat_charge_state_back;
					}
				}
				else
				{
					bat_charge_cnt = 0;
					eps_bat->bat_charge = BAT_DISCH;
				}	
			break;
		}
		v_bus_old = v_bus_cur;
		
//		if(bat_volt_ptr >= 20)
//		{ /*??????,???????????*/
//			bat_volt_ptr--;
//			if(bat_voltsqe[bat_volt_ptr] + bat_charge_voltdrop(eps_adc->v_bus) < eps_adc->v_bus )
//			{
//				bat_volt_flag = 1;
//				bat_volt_ptr = 20;
//			}/*??????,?????????*/
//			else if (bat_voltsqe[bat_volt_ptr] - bat_charge_voltdrop(eps_adc->v_bus) > eps_adc->v_bus )
//			{
//				bat_volt_flag = 0;
//				bat_volt_ptr = 40;
//			}
//			else /*??????*/
//			{/*?????20?????,??????*/
//				if(bat_volt_flag == 1)
//				{
//					if(bat_volt_ptr++ >= 40) 
//					{
//						eps_bat->bat_charge = BAT_EXT_CHARGE;
//					}
//				}
//				/*?????20?????,??????*/
//				else if(bat_volt_flag == 0)
//				{
//					if(bat_volt_ptr-- <=20)
//					{
//						eps_bat->bat_charge = BAT_DISCH;
//					}
//				}					
//				
//			}
//		}
//		else //??????
//		{
//			bat_voltsqe[bat_volt_ptr++] = eps_adc->v_bus;
//		}
		/*?????,????????,?????0,????????*/
		return bat_curr = 0;  
	}	
}

//static uint16_t bat_charge(eps_hk_adc_t *eps_adc, eps_bat_t *eps_bat )
//{
//	uint8_t i;
//	uint8_t charge_source = 0;//0=SV source;1=External source
//	uint16_t curr_hyst = 2;//mA
//	uint16_t volt_svin_hyst = 200;//mV
//	int16_t  bat_curr;     //电池电流，正表示充电电流，负表示放电电流
//	uint16_t bat_curr_in,bat_curr_out;
//	
////	bat_charge_state_t bat_charge_temp = BAT_NOCURR;
//	/*定义电池电压队列，用来判断电池的充放电状态*/
//	static uint16_t bat_voltsqe[20] = {0};
//	static uint8_t bat_volt_ptr = 0;//
//	static uint8_t bat_volt_flag = 0; 
//	/*由于充电电流来源有两个方向
//	一个是SV输入端共6路，
//	一个是外界供电充电，采用两种判断策略
//	最后依据两种策略的输出做最终判断*/
//	/*6路SV充电*/
//	for(i=0;i<SV_NUM;i++)
//	{
//		if(eps_adc->v_sv[i] > eps_adc->v_bus + volt_svin_hyst)
//		{
//			/*任何一个输入端电压大于母线电压则认为是SV source
//			跳出循环*/
//			charge_source = 0;
//			break;
//		}
//		else
//		{
//			charge_source = 1;
//		}
//	}
//	//只要上电无供电源的情况下就是放电状态
//	eps_bat->bat_charge = BAT_DISCH;
//	bat_curr_out = eps_adc->c_bus;
//	/*SV source*/
//	if(charge_source == 0)
//	{
//		bat_curr_in = eps_adc->c_sv[0]+eps_adc->c_sv[1]+eps_adc->c_sv[2]
//								+eps_adc->c_sv[3]+eps_adc->c_sv[4]+eps_adc->c_sv[5];
//		
//		if(bat_curr_in > bat_curr_out + curr_hyst)
//		{
//			eps_bat->bat_charge = BAT_SV_CHARGE;
//			bat_curr = bat_curr_in - bat_curr_out;
//		}
//		else if(bat_curr_in < bat_curr_out - curr_hyst)
//		{
//			eps_bat->bat_charge = BAT_DISCH;
//			bat_curr = bat_curr_in - bat_curr_out;
//		}
//		else
//		{
//			eps_bat->bat_charge = BAT_NOCURR;
//			bat_curr = 0;
//		}
//		return bat_curr;
//	}
//	else/*外部电源充电*/
//	{
//		if(bat_volt_ptr >= 20)
//		{ /*外部电源接上，电池电压会有一定的抬升*/
//			if(bat_voltsqe[bat_volt_ptr] + bat_charge_voltdrop(eps_adc->v_bus) < eps_adc->v_bus )
//			{
//				bat_volt_flag = 1;
//				bat_volt_ptr = 20;
//			}/*外部电源移除，电压会有一定的下降*/
//			else if (bat_voltsqe[bat_volt_ptr] - bat_charge_voltdrop(eps_adc->v_bus) > eps_adc->v_bus )
//			{
//				bat_volt_flag = 0;
//				bat_volt_ptr = 40;
//			}
//			else /*电平处于常态*/
//			{/*上升沿持续20个采样周期，认为稳定充电*/
//				if(bat_volt_flag == 1)
//				{
//					if(bat_volt_ptr++ >= 40) 
//					{
//						eps_bat->bat_charge = BAT_EXT_CHARGE;
//					}
//				}
//				/*下降沿持续20个采样周期，认为充电移除*/
//				else if(bat_volt_flag == 0)
//				{
//					if(bat_volt_ptr-- <=20)
//					{
//						eps_bat->bat_charge = BAT_DISCH;
//					}
//				}					
//				
//			}
//		}
//		else //初始填充队列
//		{
//			bat_voltsqe[bat_volt_ptr++] = eps_adc->v_bus;
//		}
//		/*外部充电时，是在地面调试阶段，返回电流值0,加热函数不会加热*/
//		return bat_curr = 0;  
//	}	
//}
/**
*@bref     battery heater function
*@detail
*@para     eps_adc     
*@         eps_bat
*@reteval  eps_heater_state    YES=heating;NO=notheating
*/
static bat_heater_state_t bat_heat(eps_hk_adc_t *eps_adc, eps_bat_t *eps_bat)
{
	int16_t  bat_temp1,bat_temp2;
	bat_temp1 = eps_adc->temp_bat[0];
	bat_temp2 = eps_adc->temp_bat[1];
	/*battery heat auto mode*/
	if(eps_bat->bat_heater_mode == 1) 
	{
		switch (eps_bat->bat_heater_status)
		{
			case NO:
			if(bat_temp1 < bat_heater_low || bat_temp2 < bat_heater_low)
			{
				if(eps_bat->bat_state > BAT_CRITICAL)
				{
					bat_heater_on();
				  eps_bat->bat_heater_status = YES;
				}
			}
				break;
			case YES:
				if(bat_temp1 > bat_heater_high || bat_temp2 > bat_heater_high)
				{
					bat_heater_off();
					eps_bat->bat_heater_status = NO;
				}
				break;
			default:
				break;
		}
	}
	
	else/* if(eps_bat->bat_heater_mode == 0)  //manual mode*/
	{
		
	}
	return eps_bat->bat_heater_status;
}
/**
*@
*@
*@
*/
static bat_state_t bat_state(eps_bat_t *eps_bat, uint16_t bat_volt)
{
	uint16_t volt_hyst = 50;//mV

//	uint16_t bat_volt;

	bat_state_t  bat_pre_state;
	
	bat_pre_state = eps_bat->bat_state;
/* 根据电压判断当前状态，为避免震荡，设定电压降低时根据设定值判断
	*电压升高时，判断值增加0.05v，即50mV的滞回量
	*************************************************************
	*/	
	switch (bat_pre_state)
	{//由满充降到正常电压范围，打开所有输入通道
		case BAT_OV:
			if(bat_volt < bat_full_volt)
			{
				eps_allin_on();
				eps_enter_normal();
				eps_bat->bat_state = BAT_NORMAL;
			}
			break;
		case BAT_NORMAL:
			//正常模式进入满充状态，关闭所有输入
			if(bat_volt > bat_full_volt + volt_hyst)
			{
				eps_allin_off();
				eps_bat->bat_state = BAT_OV;
			}
			//正常电压降到低电压，进入safe模式
			else if(bat_volt < bat_normal_volt)
			{
				eps_enter_safe();
				eps_bat->bat_state = BAT_UV;
			}
			break;
		case BAT_UV:
			//低电压升高到正常电压范围，进入normal模式
			if(bat_volt > bat_normal_volt + volt_hyst)
			{
				eps_enter_normal();
				eps_bat->bat_state = BAT_NORMAL;
			}
			//低电压降到critical电压以下，进入critical模式
			if(bat_volt < bat_critical_volt)
			{
				eps_enter_critical();
				eps_bat->bat_state = BAT_CRITICAL;
			}
			break;
		case BAT_CRITICAL:
			//critical电压升高到低电压模式
			if(bat_volt > bat_critical_volt + volt_hyst)
			{
				eps_enter_safe();
				eps_bat->bat_state = BAT_UV;
			}
			//critical电压降到接近硬件关断电压
			if(bat_volt < bat_hardoff_volt + volt_hyst)
			{
				eps_bat->bat_state = BAT_HARDOFF;
			}
			break;
		case BAT_HARDOFF:
			//接近硬件关断电压升高到critical电压
			if(bat_volt > bat_hardoff_volt + volt_hyst)
			{
				eps_bat->bat_state = BAT_CRITICAL;
			}
			break;
		default:
			break;
	}
	eps_bat->bat_status = bat_status_char[eps_bat->bat_state];
	return eps_bat->bat_state;
}
/**
*@bref     battery process function
*@detail   根据电池电压，判断当前所处状态
*          根据电池电流判断充放电状态
*          根据电池电池板温度判断电池板当前状态，是否需要加热
*          调用batSOC() 完成电池剩余电量的计算
*          调用batSOCpermanent() 完成电池容量的计算
*@para     none
*@reteval  none
*/
static void bat_data_processing(eps_hk_adc_t *eps_adc,eps_bat_t *eps_bat)
{
	uint16_t bat_volt;
	int16_t bat_curr;
	bat_volt = eps_adc->v_bus;
	/**根据电压状态判断电池状态
	*************************************************************
	*@battery voltage state
	*/
	bat_state(eps_bat, bat_volt);
	/**根据电流情况判断充放电状态
	*************************************************************
	*@battery charge state
	*/
	bat_curr = bat_charge(eps_adc,eps_bat);
	/**根据温度判断电池加热
	*0 = manual ;1 = auto
	*************************************************************
	*@battery heat function
	*/
  bat_heat(eps_adc,eps_bat);
	/**
	******************************************************************
	*@batSOC  battery SOC function
	*/
	batSOC(eps_bat,bat_volt,bat_curr);
	/**
	******************************************************************
	*@batSOCpermanent  battery SOC function
	*/	
	batSOCpermanent(eps_bat,bat_volt,bat_curr);
}























/**********************************************************************************/

