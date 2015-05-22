/*
*************************************************************************************
**
**
**
**
**************************************************************************************
*/

#ifndef __USER_PRINT_H__
#define __USER_PRINT_H__

//eps print enum para
typedef enum{
	print_eps=0,
	print_sv,
	print_bat,
	print_temp,
	print_out,
	print_mcu,
	print_bus,
	print_adc,
	
	print_help
} print_eps_t;

void UartReciveNew(void);
void printf_bat(void);
void eps_print(print_eps_t print_select);
#endif
/**********************************************************************************************/

