#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "stm32f4xx.h"


/*---------------------------------------------------------------------------------------------------*/
void bsp_InitPWM(void);


//void PWM_Update(TIM_TypeDef * TIMx,uint8_t TIM_PWMx,uint16_t PWMCCRVal);




void PWM_Updata(TIM_TypeDef * TIMx, uint16_t PWM1CCRVal,uint16_t PWM2CCRVal,uint16_t PWM3CCRVal,uint16_t PWM4CCRVal);


#endif

/**************************************end of file************************************************************/

