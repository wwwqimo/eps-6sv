#include "eps_handle.h"


extern eps_hk_adc_t eps_adc_data;
//定义全局变量 bat结构体，ppt结构体
eps_bat_t eps_bat;	
eps_ppt_t eps_ppt;		
char * bat_status_char[]={"bat full charged","bat normal voltage","bat enter safe mode",
													"bat enter critical mode","bat hard off"};	

													
void eps_enter_normal(void)
{

}
void eps_enter_safe(void)
{

}
void eps_enter_critical(void)
{

}
void eps_allin_off(void)
{

}
void eps_allin_on(void)
{

}
void bat_heater_on(void)
{

}
void bat_heater_off(void)
{

}

void eps_data_Init(void)
{
	//init eps_bat structure
	memset(&eps_bat,0,sizeof(eps_bat_t));
	eps_bat.bat_heater_mode = 0;
	eps_bat.bat_fullcap = 38480000;
	eps_bat.bat_percent = 100;
	eps_bat.bat_total_percent = 100;
	eps_bat.bat_state =BAT_NORMAL;
	eps_bat.bat_status = bat_status_char[1];
	//init eps_ppt structure
	memset(&eps_ppt,0,sizeof(eps_ppt));
	
	//
	
}
void eps_data_handling(void)
{
	bat_data_processing();
}
void bat_data_processing(void)
{
	uint16_t volt_hyst = 50;//mV
	uint16_t curr_hyst = 10;//mA
	uint16_t bat_volt;
	uint16_t bat_curr_in,bat_curr_out;
	int16_t  bat_temp1,bat_temp2;
	uint16_t bat_pre_state;
	bat_volt = eps_adc_data.v_bus;
	bat_curr_in = eps_adc_data.c_sva;
	bat_curr_out = eps_adc_data.c_bus;
	bat_temp1 = eps_adc_data.temp_bat[0];
	bat_temp2 = eps_adc_data.temp_bat[1];
	bat_pre_state = eps_bat.bat_state;
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
				eps_bat.bat_state = BAT_NORMAL;
				eps_bat.bat_status = bat_status_char[1];
			}
			break;
		case BAT_NORMAL:
			//正常模式进入满充状态，关闭所有输入
			if(bat_volt > bat_full_volt + volt_hyst)
			{
				eps_allin_off();
				eps_bat.bat_state = BAT_OV;
				eps_bat.bat_status = bat_status_char[0];
			}
			//正常电压降到低电压，进入safe模式
			else if(bat_volt < bat_normal_volt)
			{
				eps_enter_safe();
				eps_bat.bat_state = BAT_UV;
				eps_bat.bat_status = bat_status_char[2];
			}
			break;
		case BAT_UV:
			//低电压升高到正常电压范围，进入normal模式
			if(bat_volt > bat_normal_volt + volt_hyst)
			{
				eps_enter_normal();
				eps_bat.bat_state = BAT_NORMAL;
				eps_bat.bat_status = bat_status_char[1];
			}
			//低电压降到critical电压以下，进入critical模式
			if(bat_volt < bat_critical_volt)
			{
				eps_enter_critical();
				eps_bat.bat_state = BAT_CRITICAL;
				eps_bat.bat_status = bat_status_char[3];
			}
			break;
		case BAT_CRITICAL:
			//critical电压升高到低电压模式
			if(bat_volt > bat_critical_volt + volt_hyst)
			{
				eps_enter_safe();
				eps_bat.bat_state = BAT_UV;
				eps_bat.bat_status = bat_status_char[2];
			}
			//critical电压降到接近硬件关断电压
			if(bat_volt < bat_hardoff_volt + volt_hyst)
			{
				eps_bat.bat_state = BAT_HARDOFF;
				eps_bat.bat_status = bat_status_char[4];
			}
			break;
		case BAT_HARDOFF:
			//接近硬件关断电压升高到critical电压
			if(bat_volt > bat_hardoff_volt + volt_hyst)
			{
				eps_bat.bat_state = BAT_CRITICAL;
				eps_bat.bat_status = bat_status_char[3];
			}
			break;
		default:
			break;
	}
	
	/*根据电流情况判断充放电状态
	*************************************************************
	*/
	if(bat_curr_in > bat_curr_out + curr_hyst)
	{
		eps_bat.bat_charge = BAT_CHARGE;
	}
	else if(bat_curr_in < bat_curr_out - curr_hyst)
	{
		eps_bat.bat_charge = BAT_DISCH;
	}
	else
	{
		eps_bat.bat_charge = BAT_NOCURR;
	}
	/*根据温度判断电池加热
	*0 = manual ;1 = auto
	*************************************************************
	*/
	if(eps_bat.bat_heater_mode )
	{
		switch (eps_bat.bat_heater_status)
		{
			case NO:
			if(bat_temp1 <bat_heater_low )
			{
				if(eps_bat.bat_state > BAT_CRITICAL)
				{
					bat_heater_on();
				  eps_bat.bat_heater_status = YES;
				}
				
			}
				break;
			case YES:
				if(bat_temp1 > bat_heater_high )
				{
					bat_heater_off();
					eps_bat.bat_heater_status = NO;
				}
				break;
			default:
				break;
		}
	}
}
/**********************************************************************************/

