/*
*******************************************************************************************************************************
**APP MPPT
**
**
*****************************************************************************************************************************
*/

#include "app_mppt.h"

#if USER_DEBUG_EN >0u
	#define MPPT_DELAY_TIME       500
#else
	#define MPPT_DELAY_TIME       2
#endif



#define MPPT_V_Div    6.0f
#define MPPT_C_Multi  1.25f
extern float    Value_Real[NumofConv];//换算后真是采样值


//仿真数据
#if MPPT_SIM_EN >0u
//I = -V/20 +0.5;
float V_MPPT_SIM[mppt_num];
float I_MPPT_SIM[mppt_num];
#endif

#if MPPT_INC_CONDC_EN > 0u

extern MPPT_Typedef MPPT_CONV[mppt_num];

#endif

void app_MPPT(void)
{
	
	uint8_t i;
	uint16_t mppt_dacout;
	while(1)
	{
		
		#if MPPT_INC_CONDC_EN > 0u
		
		for (i=0;i<mppt_num;i++)
		{
			MPPT_CONV[i].v_cur = Value_Real[i+6]*MPPT_V_Div;
			MPPT_CONV[i].c_cur = Value_Real[i+9]/MPPT_C_Multi;
		}
		//仿真函数
		#if MPPT_INC_CONDC_SIM_EN > 0u
    MPPT_SIM();
		#endif
		
		#if USER_DEBUG_EN > 0u
		for (i=0;i<mppt_num;i++)
		{
			printf("MPPT_CONV[%d]->v_cur = %1.4f\r\n",i,MPPT_CONV[i].v_cur);
			printf("MPPT_CONV[%d]->c_cur = %1.4f\r\n",i,MPPT_CONV[i].c_cur);
			printf("MPPT_CONV[%d]->power = %2.3f\r\n",i,MPPT_CONV[i].v_cur * MPPT_CONV[i].c_cur);
		}
		#endif
		//mppt increment conductor method
		MPPT_Inc_Condc(&MPPT_CONV[0]);
		#if USER_DEBUG_EN > 0u
		for (i=0;i<mppt_num;i++)
		{
			printf("MPPT_CONV[%d]->out = %1.4f\r\n",i,MPPT_CONV[i].out);
		}
		#endif

		mppt_dacout = (uint16_t)((1.5f*(((MPPT_CONV[0].out )/ 6.0f )+ 0.8f)-2.5f)*4095/2.96f);
		printf("mppt_dacout = %4d\r\n",mppt_dacout);
		DAC_SetChannel2Data(DAC_Align_12b_R, mppt_dacout);
		#endif
		
		OSTimeDlyHMSM(0, 0, 0, MPPT_DELAY_TIME);
	}

}

//仿真函数
#if MPPT_INC_CONDC_SIM_EN > 0u
void MPPT_SIM(void)
{
	uint8_t i;

		for (i=0;i<mppt_num;i++)
		{
			V_MPPT_SIM[i] = MPPT_CONV[i].out;
			I_MPPT_SIM[i] = -V_MPPT_SIM[i]/20 + 0.5f;
		}
		//MPPT_INC_CONDC SIMULATION

		for (i=0;i<mppt_num;i++)
		{
			MPPT_CONV[i].v_cur = V_MPPT_SIM[i];
			MPPT_CONV[i].c_cur = I_MPPT_SIM[i];
		}	
}
#endif




















/**************************************************************************************************************************/

