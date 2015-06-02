#include "user_cfg.h"
#include "user_app.h"

 eps_hk_adc_t eps_adc_data;
//定义全局变量 bat结构体，ppt结构体
 eps_bat_t eps_bat;	
//define eps hk state struct avariable
 eps_hk_state_t eps_state;
 eps_ppt_t ppt_conv[SV_NUM];
 
static char *pcmdsub[]={"eps       ",
												"sv        ",
												"bat       ",
												"temp      ",
												"out       ",
												"mcu       ",
												"bus       ",
												"adc       ",
												"help      ",
												"iap       ",
												"IAP       "};

												
void UartReciveNew(void)
{
	uint32_t i,ptr = 0;
	uint32_t cmdsize;
	int32_t data ;
	static print_eps_t print_eps_pre_sub = print_help;
	uint8_t flag = 0; //flag = :mean find "eps";flag = 2:mean find subcmd
//	char cmdmain[3];
	char cmdsub[11] = {'\0'};
//	char cmdpara[5];
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
				flag = 2;//find sub command ending
				break;
			}				
			if (ptr > 10)
			{
				flag = 0;
				break;
			}
		}while(1);
	}
	
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
					eps_print((print_eps_t)i);//match sub command
					print_eps_pre_sub = (print_eps_t)i;
					break;
				}
				else if (i == sizeof(pcmdsub)/sizeof(char *)-1)
				{
					printf("your subcmd is \"%s\"\r\n",cmdsub);
					printf("sub command error!!\r\n");
					printf("type \"eps help\" for more commands\r\n");
				}
			}
		}
		else if(flag < 2)
		{
//			eps_print(print_help);
			printf("type \"eps help\" for more commands\r\n");			
			printf("The previous command is called\r\n");

			eps_print(print_eps_pre_sub);
		}
		
	uart_fifo_clear(&uart_fifo);
}
void printf_help(void)
{
	printf("|****Eps Command****Brief****************************************|\r\n");
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
	uint8_t i;
	float power_inall = 0;
	float power_in = 0;
	float efficiency = 0;
	printf("|****Eps SV 6 *--Voltage*--Current*--PowerIn*--Efficiency*************************************|\r\n");
	for(i=0;i<SV_NUM;i++){
	power_in = (float)ppt_conv[i].curr * ppt_conv[i].volt/1000000;
	printf("| eps sv[%1d]     %5.3fV     %4dmA    %5.3fW\r\n",i,((float)ppt_conv[i].volt)/1000,ppt_conv[i].curr,power_in);
	power_inall += power_in;	
	}
	printf("| power_inall                         %5.3fW\r\n",power_inall);
	efficiency = (float)eps_adc_data.c_sva * eps_adc_data.v_bus/1000000/power_inall;
	printf("|********************************************      %4.2f%%\r\n",efficiency*100);
}

void printf_bat(void)
{
	printf("|****Eps Battery All States***********************************|\r\n");
	printf("| eps bat status :                 %s\r\n",eps_bat.bat_status);
	printf("| eps bat voltage :                %4dmV\r\n",eps_adc_data.v_bus);
	printf("| eps bat current in :             %4dmA\r\n",eps_adc_data.c_sva);
	printf("| eps bat current out :            %4dmA\r\n",eps_adc_data.c_bus);
	printf("| eps bat current :                %4dmA\r\n",eps_adc_data.c_sva - eps_adc_data.c_bus);	
	printf("| eps bat consume :                %4dmW\r\n",eps_adc_data.c_bus *eps_adc_data.v_bus/1000);
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
	printf("| eps bat temp  :                 %2d degc  %2d degc\r\n",eps_adc_data.temp_bat[0],eps_adc_data.temp_bat[1]);
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
	printf("|****Tempture***********************************************************|\r\n");
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
	uint16_t i;
	printf("|****Eps Outputs*******************************************************|\r\n");
	printf("| Eps Outputs Total Fault %5d\r\n",eps_state.out_faults);
	printf("| position  num  state  ocp_count  fault_count  Ton  Toff\r\n");
	for(i=0;i<REG_NUM+UREG_NUM;i++)
	{
		switch (i)
		{
			case 0: printf("| H1-47");
			break;
			case 1: printf("| H1-48");
			break;
			case 2: printf("| H1-50");
			break;
			case 3: printf("| EXT-1");
			break;
			case 4: printf("| EXT-2");
			break;
			case 5: printf("| H1-49");
			break;
			case 6: printf("| H1-51");
			break;
			case 7: printf("| H1-52");
			break;
			case 8: printf("| H2-99");
		}
		printf("     %1d    %1d  %5d       %5d          %2d   %2d\r\n",
		i,eps_state.out_state[i],eps_state.out_ocp[i],eps_state.out_fault[i],eps_state.out_Ton[i],eps_state.out_Toff[i]);
	}
	printf("| H2-100\r\n");
	printf("| output state: 0=OFF,1=ON,2=ERR,3=SFTRY,4=HDTRY,5=TRYOK,6=HDERR\r\n");
	printf("|*********************************************************************|\r\n");
}
void printf_mcu(void)
{
	
}
void printf_bus(void)
{
	printf("|****Eps Bus*************************************************************************|\r\n");
	printf("|");
}

void printf_adc(void)
{
	printf("|****Eps ADC All Datas*********************************************************************|\r\n");
	printf("| C_SV0 %4d mA, C_SV1 %4d mA, C_SV2 %4d mA, C_SV3 %4d mA, C_SV4 %4d mA, C_SV5 %4d mA |\r\n",
	eps_adc_data.c_sv[0],eps_adc_data.c_sv[1],eps_adc_data.c_sv[2],eps_adc_data.c_sv[3],eps_adc_data.c_sv[4],eps_adc_data.c_sv[5]);
	printf("| V_SV0 %4d mV, V_SV1 %4d mV, V_SV2 %4d mV, V_SV3 %4d mV, V_SV4 %4d mV, V_SV5 %4d mV |\r\n",
	eps_adc_data.v_sv[0],eps_adc_data.v_sv[1],eps_adc_data.v_sv[2],eps_adc_data.v_sv[3],eps_adc_data.v_sv[4],eps_adc_data.v_sv[5]);
	printf("| C_SVA %4d mA, C_BUS %4d mA, V_BUS %4d mV, V_5V  %4d mV, V_3V3 %4d mV                |\r\n",
	eps_adc_data.c_sva,eps_adc_data.c_bus,eps_adc_data.v_bus,eps_adc_data.v_5v,eps_adc_data.v_3v);
	printf("| C_OR0 %4d mA, C_OR1 %4d mA, C_OR2 %4d mA, C_OR3 %4d mA, C_OR4 %4d mA                |\r\n",
	eps_adc_data.c_out[0],eps_adc_data.c_out[1],eps_adc_data.c_out[2],eps_adc_data.c_out[3],eps_adc_data.c_out[4]);
	printf("| C_OU0 %4d mA, C_OU1 %4d mA, C_OU2 %4d mA, C_OU3 %4d mA                               |\r\n",
	eps_adc_data.c_out[5],eps_adc_data.c_out[6],eps_adc_data.c_out[7],eps_adc_data.c_out[8]);
	printf("| ET0 %4d degc, ET1 %4d degc, ET2 %4d degc, ET3 %4d degc, BT0 %4d degc, BT1 %4d degc |\r\n",
	eps_adc_data.temp_eps[0],eps_adc_data.temp_eps[1],eps_adc_data.temp_eps[2],eps_adc_data.temp_eps[3],eps_adc_data.temp_bat[0],eps_adc_data.temp_bat[1]);

}
/*
** eps print function 
** print_select  print select para  
**
*/
extern void ExecutingApplication(void);
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
		case iap: case IAP:
			ExecutingApplication();
		default:
			break;
	}
}



/*********************************************************************************/


