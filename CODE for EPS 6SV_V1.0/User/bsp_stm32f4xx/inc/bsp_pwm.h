#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "stm32f4xx.h"


/*---------------------------------------------------------------------------------------------------*/
void bsp_InitPWM(void);



uint8_t PWM_Updata(uint16_t PWM_CCRVal, uint8_t pwm_chan);


#endif

/**************************************end of file************************************************************/

