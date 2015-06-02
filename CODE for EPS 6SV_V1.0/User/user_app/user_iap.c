/*
********************************************************************************************
**
**
**
********************************************************************************************
*/
#include "stm32f4xx.h"


typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;

#define APPLICATION_ADDRESS   (uint32_t)0x08000000 
void ExecutingApplication(void);


void ExecutingApplication(void)
{
	/* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    { 
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
      Jump_To_Application();
    }
}



/**********************************************************************************************/
