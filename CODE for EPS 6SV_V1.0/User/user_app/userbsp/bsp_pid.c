/**************************************************************************************************/
/*file name:     algorithm_pid.c
**file function: pid function
**created time:  2014.12.17
**creator:       leixiaoxiong
**version:       v0.1
*/
//******************includes******************************************************************
#include "bsp_pid.h"




/**
*****************************************************************************************
**��������  InitAll_IncPID
**����˵��������ʽPID���Ƴ�ʼ��
**�βΣ�    none
**����ֵ��  none
****************************************************************************************
**/
//void bsp_IncPID_Init(void)
//{
//	uint8_t i;
//	for(i=0;i<SV_NUM;i++)
//	{
//		Init_IncPID( &pid_ppt_para[i]);
//		Init_IncPIDvar( &pid_ppt_var[i]);
//	}
//}

/**
*****************************************************************************************
**��������  Init_IncPIDvar
**����˵��������ʽPID���Ʋ�����ʼ��
**�βΣ�    PID_Typedef *Svar       ��PID������������ṹ��
**����ֵ��  none
****************************************************************************************
**/
void Init_IncPIDvar(PID_Typedef *Svar)
{
	memset(Svar,0,sizeof(PID_Typedef));

}
/**
*****************************************************************************************
**��������  Init_IncPID
**����˵��������ʽPID���Ʋ�����ʼ��
**�βΣ�    arm_pid_instance_f32 *S: PID���Ʋ����ṹ��
**����ֵ��  none
****************************************************************************************
**/
void Init_IncPID(arm_pid_instance_f32 *S)
{
	S->Kp = 0;
	S->Ki = 0;
	S->Kd = 0;
	arm_pid_init_f32(S,1);	
}
/**
*******************************************************************************************
**��������   Cal_IncPID_Minus
**����˵���� ����ʽPID����,�ڲ�ʵ��Ϊ����
**�βΣ�     arm_pid_instance_f32 *S: PID���Ʋ����ṹ��
**           PID PID_ST *Svar       ��PID������������ṹ��
**           float SampleValue      : ��������
**����ֵ��   ����ʽPID���ֵ
****************************************************************************************
**/
float Cal_IncPID_Minus(arm_pid_instance_f32 *S, PID_Typedef *Svar,float SampleValue)
{
	S->state[0] = Svar->SetValue - SampleValue;//���
	S->state[1] += S->state[0];//����
	S->state[2] = S->state[0] - Svar->LastErr;//���΢��
	
	Svar->LastErr = S->state[0];
//	Cal_IncPIDpara(S, Svar);
	
  Svar->Output -= S->Kp * S->state[0] + S->Ki * S->state[1] + S->Kd * S->state[2];
	
	Check_IncPIDout(Svar);
	return (uint16_t)Svar->Output;
}
/**
*******************************************************************************************
**��������   Cal_IncPID_Plus
**����˵���� ����ʽPID����,�ڲ�ʵ��Ϊ�ӷ�
**�βΣ�     arm_pid_instance_f32 *S: PID���Ʋ����ṹ��
**           PID PID_ST *Svar       ��PID������������ṹ��
**           float SampleValue      : ��������
**����ֵ��   ����ʽPID���ֵ
****************************************************************************************
**/
float Cal_IncPID_Plus(arm_pid_instance_f32 *S, PID_Typedef *Svar,float SampleValue)
{
	S->state[0] = Svar->SetValue - SampleValue;//���
	S->state[1] += S->state[0];//����
	S->state[2] = S->state[0] - Svar->LastErr;//���΢��
	
	Svar->LastErr = S->state[0];
//	Cal_IncPIDpara(S, Svar);
	
  Svar->Output += S->Kp * S->state[0] + S->Ki * S->state[1] + S->Kd * S->state[2];
	
	Check_IncPIDout(Svar);
	return (uint16_t)Svar->Output;
}
/*
****************************************************************************************
**��������  Cal_IncPIDpara
**����˵����PID���Ʋ�������
**�βΣ�    arm_pid_instance_f32 *S: PID���Ʋ����ṹ��
**          PID PID_ST *Svar       ��PID������������ṹ��
**����ֵ��  none
*************************************************************************************
*/
void Cal_IncPIDpara(arm_pid_instance_f32 *S, PID_Typedef *Svar)
{
	if (S->state[0]> Svar->ZeroErrMax)
	{
		
	}
	else if (S->state[0]< Svar->ZeroErrMin)
	{
		
	}
	
	
}
/**
*****************************************************************************************
**��������   Check_IncPIDout
**����˵���� PID�����ֵ���
**�βΣ�     PID_ST *Svar ��PID������������ṹ��
**����ֵ��   none
****************************************************************************************
**/
void Check_IncPIDout(PID_Typedef *Svar)
{
	if (Svar->Output > Svar->OutMax)
	{
		Svar->Output = Svar->OutMax;
	}
	else if (Svar->Output < Svar->OutMin)
	{
	  Svar->Output = Svar->OutMin;
	}
}


/*****************************END OF FILE***********************************************************************/
