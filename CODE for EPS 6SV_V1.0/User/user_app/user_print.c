#include "user_cfg.h"
#include "user_app.h"

//声明 eps_adc_data结构体
extern eps_hk_adc_t eps_adc_data;

//声明全局变量 bat结构体，ppt结构体
extern eps_bat_t eps_bat;	
extern eps_ppt_t eps_ppt;

extern uart_fifo_t uart_fifo;
static char *pcmdsub[]={"eps       ",
												"sv        ",
												"bat       ",
												"temp      ",
												"out       ",
												"mcu       ",
												"bus       ",
												"adc       ",
												"help      "};

void UartReciveNew(void)
{
	uint32_t i,ptr = 0;
	uint32_t cmdsize;
	int32_t data ;
	uint8_t flag = 0; //flag = :mean find "eps";flag = 2:mean find subcmd
	char cmdmain[3];
	char cmdsub[11] = {'\0'};
	char cmdpara[5];
	//get the size of the buffer used
	cmdsize = uart_fifo_used(&uart_fifo);
	//get a data from the uart buffer  
	data = uart_fifo_get(&uart_fifo);
	//check if the uart buffer overflow
	if(data<0)
	{
		printf("uart buffer overflow\r\n");
		return;
	}
	
	while(((char)data =='e'||(char)data == 'E')&&flag == 0)
	{
		data = (char)uart_fifo_get(&uart_fifo);
		if((char)data == 'p'||(char)data == 'P')
		{
			data = (char)uart_fifo_get(&uart_fifo);
			if((char)data == 's'||(char)data == 'S')
			{
				data = uart_fifo_get(&uart_fifo);
				if(data == 0x20)
				{
					flag = 1;//find "eps"
				}
				
			}
		}
	}

	
	if(flag == 1)
	{
//		printf("eps ");//test
		do{
			data = uart_fifo_get(&uart_fifo);
//			printf("%c",(char)data);//test
			cmdsub[ptr++] = (char)data;
			if((char)data == '\r' || (char)data == '\n')
			{
				flag = 2;
				break;
			}				
			if (ptr > 10)
			{
				flag = 0;
				break;
			}
		}while(1);
		if(flag == 2)
		{
			for (i=(ptr-1);i<10;i++)
			{
				cmdsub[i] = 0x20;//fill the remained array units with space
			}

			for(i=0;i<sizeof(pcmdsub)/sizeof(char *);i++)
			{
				if(strcmp(pcmdsub[i],cmdsub) == 0)
				{
					eps_print((print_eps_t)i);
				}
			}
		}
		
		
	}
	if(flag < 2)
	{
		eps_print(print_help);
	}
	else if(flag == 2)
	{
	
	}
	uart_fifo_clear(&uart_fifo);
}
void printf_help(void)
{
	printf("|****eps command****brief*****************************************|\r\n");
	printf("|    eps eps        call eps main state\r\n");
	printf("|    eps sv         call sv state\r\n");
	printf("|    eps bat        call bat state\r\n");
	printf("|    eps temp       call temptures state\r\n");
	printf("|    eps out        call outputs state\r\n");
	printf("|    eps mcu        call mcu state\r\n");
	printf("|    eps bus        call bus state\r\n");
	printf("|    eps adc        call all adc datas\r\n");
	printf("|    eps help       call eps help instructions\r\n");
	printf("|****************************************************************|\r\n");
}
void printf_eps(void)
{
	
}
void printf_sv(void)
{
	
}

void printf_bat(void)
{
	printf("|****eps battery all states***********************************|\r\n");
	printf("| eps bat status :                 %s\r\n",eps_bat.bat_status);
	printf("| eps bat voltage :                %4dmV\r\n",eps_adc_data.v_bus);
	printf("| eps full capacity:               %4.2fWHr\r\n",((float)(eps_bat.bat_fullcap)/1000000));
	printf("| eps current capcity:             %3d%%\r\n",eps_bat.bat_total_percent);
	printf("| eps current power remained:      %3d%%\r\n",eps_bat.bat_percent);
	if(eps_bat.bat_heater_mode)
	{
		printf("| eps heater mode:                auto\r\n");
	}
	else
	{
		printf("| eps heater mode:                manual\r\n");
	}
	if(eps_bat.bat_heater_status == YES)
	{
		printf("| eps heater status:              battery heating\r\n");
	}
	else
	{
		printf("| eps heater status:              battery  not heating\r\n");
	}
	
	printf("| eps bat off count number:       %4d \r\n",eps_bat.bat_off_count);
	printf("|*************************************************************|\r\n");
}
void printf_temp(void)
{
	printf("|****tempture***********************************************************|\r\n");
	printf("temp0           %2d degc  on the top left corner of eps,top side\r\n",eps_adc_data.temp_eps[0]);
	printf("temp1           %2d degc  on the lower left corner of eps,top side\r\n",eps_adc_data.temp_eps[1]);
	printf("temp2           %2d degc  on the lower right corner of eps,bottom side\r\n",eps_adc_data.temp_eps[2]);
	printf("temp3           %2d degc  on the middle up part of eps,top side\r\n",eps_adc_data.temp_eps[3]);
	printf("bat_temp0       %2d degc  on the lower left corner of battery,top side\r\n",eps_adc_data.temp_bat[0]);
	printf("bat_temp1       %2d degc  on the lower right corner of battery,top side\r\n",eps_adc_data.temp_bat[1]);
	printf("|***********************************************************************|\r\n");
}
	
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
		case print_help:
			printf_help();
			break;
		default:
			break;
	}
}



/*********************************************************************************/


