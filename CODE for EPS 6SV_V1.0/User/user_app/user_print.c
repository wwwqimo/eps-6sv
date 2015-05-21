#include "user_cfg.h"
#include "user_app.h"

//声明 eps_adc_data结构体
extern eps_hk_adc_t eps_adc_data;

//声明全局变量 bat结构体，ppt结构体
extern eps_bat_t eps_bat;	
extern eps_ppt_t eps_ppt;


void printf_eps(void)
{
	
}
void printf_sv(void)
{
	
}

void printf_bat(void)
{
	printf("|****eps battery all states***********************************|");
	printf("| eps bat status :%s",eps_bat.bat_status);
	printf("| eps full capacity: %4.2fWHr",(float)eps_bat.bat_fullcap/1000000);
	printf("| eps current capcity: %3d%%",eps_bat.bat_total_percent);
	printf("| eps current power remained: %3d%%",eps_bat.bat_percent);
	if(eps_bat.bat_heater_mode)
	{
		printf("| eps heater mode: auto");
	}
	else
	{
		printf("| eps heater mode: manual");
	}
	if(eps_bat.bat_heater_status == YES)
	{
		printf("| eps heater status: battery heating");
	}
	else
	{
		printf("| eps heater status: battery  not heating");
	}
	
	printf("| eps bat off count number: %4d ",eps_bat.bat_off_count);
}
void printf_temp(void)
{}
	
void printf_out(void)
{

}
void printf_mcu(void)
{

}
void printf_bus(void)
{

}

void printf_adc(void)
{
	printf("|****eps adc all datas*********************************************************************|\r\n");
	printf("| C_SV0 %4d mA, C_SV1 %4d mA, C_SV2 %4d mA, C_SV3 %4d mA, C_SV4 %4d mA, C_SV5 %4d mA |\r\n",
	eps_adc_data.c_sv[0],eps_adc_data.c_sv[1],eps_adc_data.c_sv[2],eps_adc_data.c_sv[3],eps_adc_data.c_sv[4],eps_adc_data.c_sv[5]);
	printf("| V_SV0 %4d mV, V_SV1 %4d mV, V_SV2 %4d mV, V_SV3 %4d mV, V_SV4 %4d mV, V_SV5 %4d mV |\r\n",
	eps_adc_data.v_sv[0],eps_adc_data.v_sv[1],eps_adc_data.v_sv[2],eps_adc_data.v_sv[3],eps_adc_data.v_sv[4],eps_adc_data.v_sv[5]);
	printf("| C_SVA %4d mA, C_BUS %4d mA, V_BUS %4d mV, V_5V  %4d mV, V_3V3 %4d mV                |\r\n",
	eps_adc_data.c_sva,eps_adc_data.c_bus,eps_adc_data.v_bus,eps_adc_data.v_5v,eps_adc_data.v_3v);
	printf("| C_OR0 %4d mA, C_OR1 %4d mA, C_OR2 %4d mA, C_OR3 %4d mA, C_OR4 %4d mA                |\r\n",
	eps_adc_data.c_reg[0],eps_adc_data.c_reg[1],eps_adc_data.c_reg[2],eps_adc_data.c_reg[3],eps_adc_data.c_reg[4]);
	printf("| C_OU0 %4d mA, C_OU1 %4d mA, C_OU2 %4d mA, C_OU3 %4d mA                               |\r\n",
	eps_adc_data.c_ureg[0],eps_adc_data.c_ureg[1],eps_adc_data.c_ureg[2],eps_adc_data.c_ureg[3]);
	printf("| ET0 %4d degc, ET1 %4d degc, ET2 %4d degc, ET3 %4d degc, BT0 %4d degc, BT1 %4d degc |\r\n",
	eps_adc_data.temp_eps[0],eps_adc_data.temp_eps[1],eps_adc_data.temp_eps[2],eps_adc_data.temp_eps[3],eps_adc_data.temp_bat[0],eps_adc_data.temp_bat[1]);

}
/*
** eps print function 
** print_select  print select para  
**
*/
void eps_print(print_eps_t print_select)
{
	switch (print_select)
	{
		case print_eps: 
			printf_eps();
			break;
		case print_sv: 
			printf_sv();
			break;
		case print_bat:
			printf_bat();
			break;
		case print_temp:
			printf_temp();
			break;
		case print_out:
			printf_out();
			break;
		case print_mcu:
			printf_mcu();
			break;
		case print_bus:
			printf_bus();
			break;
		case print_adc:
			printf_adc();
			break;
		default:
			break;
	}
}



/*********************************************************************************/


