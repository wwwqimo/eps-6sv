/*
***********************************************************************************************************************
**APP MPPT
**
**
************************************************************************************************************************
*/

#ifndef __APP_MPPT_H__
#define __APP_MPPT_H__

#include "user_app.h"
#include "user_cfg.h"



























void app_MPPT(void);
#if MPPT_INC_CONDC_SIM_EN > 0u
    void MPPT_SIM(void);
#endif


#endif 
/*****************************************************************************************************************************/

