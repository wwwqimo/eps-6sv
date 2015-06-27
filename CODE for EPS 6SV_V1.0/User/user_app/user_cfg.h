#ifndef __USER_CFG_H__
#define __USER_CFG_H__

#include "stm32f4xx.h"

#define  USER_DEBUG_EN      1u
/**
*@defconfig      eps gpio define config
*@detail    0 not used 1 used
*********************************************************************************************
*/
#define  EPS_PS_USED        0u  //eps battery power switch 

#define  EPS_HS_USED        1u  //eps battery heater 

#define  EPS_PSW_USED       0u  //eps mcu power switch  

#define  EPS_DS_USED        0u  //eps deployment switch
 
#define  EPS_FAULT_USED     1u  //eps ocp outputs hardware default function 

/*mppt method define*/

#define  MPPT_INC_CONDC_EN  1u

#define  MPPT_SIM_EN        0u   //mppt simulation enable

#define  MPPT_INC_CONDC_SIM_EN  0u




#endif /*user_cfg.h*/
/*******************************************************************************************************************************/
