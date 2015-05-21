/*
********************************************************************************************************************
**dac
**
**
**
*************************************************************************************************************************
*/
#include "bsp_f4dac.h"

#include "user_cfg.h"
#include "user_app.h"
#include <math.h>


#define Vout_Div         4.3    //�����ѹ������ѹϵ��
#define VFB_Div          2    //������ѹ��ѹϵ��
#define DACout_Div       1.0f  //DAC��������
#define VFB              0.8f  //�������������Ż�׼��ѹ
#define DAC_REF          2.96f  //DAC�ο���ѹ
#define DAC_FullScale    4095 //DAC����
/*
*******************************************************************************************************
**DAC �����ѹ���ã�
**chan Ϊѡ��ͨ��
**v_value Ϊ�����ѹֵ
**
********************************************************************************************************
*/
void DAC_Out(float v_value, uint16_t chan)
{
	float v_set;
	uint16_t DAC_set;
	
	/*
	**���ݵ���������(Vout/V_Div+V_set)/2 = VFB*VFB_Div
	**V_set = VFB*VFB_Div*2-Vout/V_Div
	*/
	//���DAC�趨ֵ
	v_set = VFB*VFB_Div*2.0f-v_value/Vout_Div;
	if (v_set > DAC_REF)
	{
		v_set = DAC_REF;
	}
	if (v_set < 0 )
	{
		v_set = 0;
	}
	DAC_set = (uint16_t)(DAC_FullScale * v_set / DAC_REF);
	if(chan == 0)
	{
		DAC_SetChannel1Data(DAC_Align_12b_R, DAC_set);
	}
	if(chan == 1)
	{
		DAC_SetChannel2Data(DAC_Align_12b_R, DAC_set);
	}
	#if USER_DEBUG_EN >0u
	printf("v_set[%d] =   %f\r\n",chan,v_set);
	printf("DAC_set[%d] = %4X\r\n",chan,DAC_set);
	#endif
}















/******************************************************************************************************************/

