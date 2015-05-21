/*
*************************************************************************************************************
**
**
**
**
***************************************************************************************************************
*/
#include "bsp_SolarArraySim.h"

#include <math.h>
#include <string.h>
/*
*********************************************************************
**%I=Isc{1-C1[exp(V/(C2*Voc))-1]}  //I-V����
**% C2=(Vm/Voc-1)/(log(1-Im/Isc));
**% C1=(1-Im/Isc)*exp(-Vm/(C2*Ioc));
**
**%��������
**s=1350w/m2;
**t=25;
**%��ز���
**voc=2.65v;
**vm=2.35v;
**isc=0.4224A;
**im=0.396A;
**tref=25��;
**sref=1350;
**%ϵ������
**a=0.0025;
**b=0.5;
**c=0.0028;
**detS=(S/Sref)-1;
**detT=T-Tref;
**%��������
**Iscp=Isc*(1+a*detT)*S/Sref;
**Imp=Im*(1+a*detT)*S/Sref;
**Vocp=Voc*(1-c*detT)*(log(b*detS)+1);
**Vmp=Vm*(1-c*detT)*(log(b*detS)+1);
**%��ʽ����
C2=(Vmp/Vocp-1)/(log(1-Imp/Iscp));
C1=(1-Imp/Iscp)*exp(-Vmp/(C2*Vocp));
I=Iscp*(1-C1*(exp(V/(C2*Vocp))-1));
*********************************************************************
*/


PV_Typedef      PV_model[model_num];
PVpara_Typedef  PV_para[model_num];
PVnum_Typedef   PV_num[chan_num];


/*****�ֲ���������*******************************************************************************/
static float detS(PV_Typedef *PV);
static float detT(PV_Typedef *PV);	
static float Iscp(PV_Typedef *PV);
static float Imp(PV_Typedef *PV);
static float Vocp(PV_Typedef *PV);
static float Vmp(PV_Typedef *PV);
static float C2(PV_Typedef *PV);
static float C1(PV_Typedef *PV);
static float I(PV_Typedef *PV,float V);
/*
*************************************************************************************************
**PV��ʼ������
**˵������������Ĳ���������copy��PV�ṹ����ȥ�������������ɼ���
**
**
**************************************************************************************************
*/
void bsp_PV_Init(void)
{ 
	uint8_t i;
	memset(PV_model,0,sizeof(PV_Typedef)*model_num);
	for(i=0;i<model_num;i++)
	{
		
		PV_para[i].a = 0.0025;
		PV_para[i].b = 0.5000;
		PV_para[i].c =0.0028;
		PV_para[i].Voc = 2.6500;
		PV_para[i].Vm =2.3500;
		PV_para[i].Isc = 0.4224;
		PV_para[i].Im = 0.3960;
		PV_para[i].Tref =25.0000;
		PV_para[i].Sref = 1350.0000;
		PV_para[i].S = 1350.0000;
		PV_para[i].T =25.0000;
		PV_para[i].flag =1;
		
		PV_Change(&PV_model[i],&PV_para[i]);
	}
	for (i=0;i<chan_num;i++)
	{
		PV_num[i].n_p =2;
		PV_num[i].n_s =5;
		PV_num[i].model_type = 0;//Ĭ��ѡ��0��ģ��
	}
}
/*
**********************************************************************************************
**PV �����ı亯�� �������������ı�ͻ������Ƭ�����ı�
**
**
**********************************************************************************************
*/
void PV_Change(PV_Typedef *PV,PVpara_Typedef *SCpara)
{
	PV_ChangeType(PV,SCpara);
	PV_ChangeCondition(PV,SCpara);
}
/*
**********************************************************************************************
**PV ���������ı亯��
**
**
**********************************************************************************************
*/
void PV_ChangeCondition(PV_Typedef *PV,PVpara_Typedef *SCpara)
{
	memcpy((void*)&PV->PVpara.flag,(void*)&SCpara->flag,sizeof(PVpara_Typedef));
}
/*
**********************************************************************************************
**PV ���Ƭ�����ı亯��
**
**
**********************************************************************************************
*/
void PV_ChangeType(PV_Typedef *PV,PVpara_Typedef *SCpara)
{
	memcpy((void*)&PV->PVpara.flag,(void*)&SCpara->flag,sizeof(PVpara_Typedef));
}
//��������
static float detS(PV_Typedef *PV)
{
	return ((PV->PVpara.S / PV->PVpara.Sref)-1);
}
static float detT(PV_Typedef *PV)
{
	return (PV->PVpara.T - PV->PVpara.Tref);
}

static float Iscp(PV_Typedef *PV)
{
	return (PV->PVpara.Isc*(1.0f+PV->PVpara.a*detT(PV))*(PV->PVpara.S / PV->PVpara.Sref));
}
static float Imp(PV_Typedef *PV)
{
	return (PV->PVpara.Im*(1.0f+PV->PVpara.a*detT(PV))*(PV->PVpara.S / PV->PVpara.Sref));
}
static float Vocp(PV_Typedef *PV)
{
	return (PV->PVpara.Voc*(1.0f-PV->PVpara.c*detT(PV))*(log(PV->PVpara.b*detS(PV))+1.0f));
}
static float Vmp(PV_Typedef *PV)
{
	return (PV->PVpara.Vm*(1.0f-PV->PVpara.c*detT(PV))*(log(PV->PVpara.b*detS(PV)))+1.0f);
}
static float C2(PV_Typedef *PV)
{
	return ((Vmp(PV)/Vocp(PV)-1.0f)/(log(1.0f-Imp(PV)/Iscp(PV))));
}

static float C1(PV_Typedef *PV)
{
	return ((1.0f-Imp(PV)/Iscp(PV))*exp(-Vmp(PV)/(C2(PV)*Vocp(PV))));
}

static float I(PV_Typedef *PV,float V)
{
	return (Iscp(PV)*(1.0f-C1(PV)*(exp(V/(C2(PV)*Vocp(PV)))-1.0f)));
}
/*
**************************************************************************************
**�������ɺ���
**���ݽṹ���в����������߱��洢�ڽṹ����IV[][]������
**
***************************************************************************************
*/
uint8_t IV_generate(PV_Typedef *PV)
{
	uint16_t i;
	float iv=0;
	float Amp;
	float temp1,temp2;
	uint8_t ppt_flag = 0,prev_flag = 0,nxt_flag = 0;
	float iv_step = PV->PVpara.Voc/(PVdiv_num-1);
	for (i=0;i<PVdiv_num;i++)
	{
		Amp = I(PV,iv);
		if (ppt_flag == 0)
		{
		//Ѱ������ʵ�
			temp1 =Amp*iv;
    
			if(temp1<temp2)
			{
			
				PV->Power_mppt = temp2;
				PV->position_mppt = i-1;
			
				ppt_flag =1;
			}
			temp2 = temp1;
		}
		//����ǰ������Ͷ�·����֮��С��10%�Ķ�·����������Ϊ��������
		//ȷ��ֱ�߲���1�ֽ��position_prev
		if (prev_flag == 0)
		{
			if ((I(PV,0) - Amp) >  I(PV,0) * 0.1f )
			{
				PV->position_prev = i;
				prev_flag = 1;
			}
		}
		//ȷ��ֱ�߲���2�ֽ��position_nxt
		if (nxt_flag == 0)
		{
			if (((PV->PVpara.Voc - iv)*(I(PV,(PV->PVpara.Voc)*0.95f))/(0.05f *(PV->PVpara.Voc)) - I(PV,iv))
					< 0.1f * (PV->PVpara.Isc))
			{
				PV->position_nxt = i;
				nxt_flag = 1;
			}
	  }
		if (PV->position_prev > PV->position_mppt)
		{
			PV->position_prev = (PV->position_mppt) - 100;
		}
		if (PV->position_nxt < PV->position_mppt)
		{
			PV->position_nxt = (PV->position_mppt)+100;
		}
		PV->IV[0][i] = Amp;
		PV->IV[1][i] = iv;
		//��ʼ��ǰ���ݵ�PV->position_current Ϊ����ʵ�
		PV->position_current = PV->position_mppt;
		iv += iv_step;
	}
	return 0;
}

/*
**SAS_EN
**chan   ѡ���ͨ��
**State  ͨ��״̬
**
*/

void SAS_EN(uint16_t chan, FunctionalState State)
{
	switch (chan)
	{
		case 0:
		if(State == ENABLE)
		{
			GPIO_SetBits(GPIO_PORT_SAS_EN0, GPIO_PIN_SAS_EN0);
		}
		else
		{
			GPIO_ResetBits(GPIO_PORT_SAS_EN0, GPIO_PIN_SAS_EN0);
		}
		break;
		case 1:
		if(State == ENABLE)
		{
			GPIO_SetBits(GPIO_PORT_SAS_EN1, GPIO_PIN_SAS_EN1);
		}
		else
		{
			GPIO_ResetBits(GPIO_PORT_SAS_EN1, GPIO_PIN_SAS_EN1);
		}
		break;
		case 2:
		if(State == ENABLE)
		{
			GPIO_SetBits(GPIO_PORT_SAS_EN2, GPIO_PIN_SAS_EN2);
		}
		else
		{
			GPIO_ResetBits(GPIO_PORT_SAS_EN2, GPIO_PIN_SAS_EN2);
		}
		break;
		default:
			break;
	}
}




/*************************************************************************************************************/
