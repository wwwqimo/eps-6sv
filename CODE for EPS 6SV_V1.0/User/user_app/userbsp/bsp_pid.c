/**************************************************************************************************/
/*file name:     algorithm_pid.c
**file function: pid function
**created time:  2014.12.17
**creator:       leixiaoxiong
**version:       v0.1
*/
//******************includes******************************************************************
#include "bsp_pid.h"
#include "arm_math.h"
#include "stm32f4xx.h"

//pid 参数实例化
arm_pid_instance_f32     PID_BUCK_SAS_V;
//pid 数据实例化
PID_Typedef              PID_BUCK_SAS_var_V;

static void Init_IncPID(arm_pid_instance_f32 *S);
static void Init_IncPIDvar(PID_Typedef *S);

void Cal_IncPIDpara(arm_pid_instance_f32 *S, PID_Typedef *Svar);
void Check_IncPIDout(PID_Typedef *Svar);
/**
*****************************************************************************************
**函数名：  InitAll_IncPID
**函数说明：增量式PID控制初始化
**形参：    none
**返回值：  none
****************************************************************************************
**/
void bsp_IncPID_Init(void)
{
	Init_IncPID( &PID_BUCK_SAS_V );
	Init_IncPIDvar( &PID_BUCK_SAS_var_V );
}

/**
*****************************************************************************************
**函数名：  Init_IncPIDvar
**函数说明：增量式PID控制参数初始化
**形参：    PID_Typedef *Svar       ：PID输入输出参数结构体
**返回值：  none
****************************************************************************************
**/
static void Init_IncPIDvar(PID_Typedef *Svar)
{
	memset(Svar,0,sizeof(PID_Typedef));

}
/**
*****************************************************************************************
**函数名：  Init_IncPID
**函数说明：增量式PID控制参数初始化
**形参：    arm_pid_instance_f32 *S: PID控制参数结构体
**返回值：  none
****************************************************************************************
**/
static void Init_IncPID(arm_pid_instance_f32 *S)
{
	S->Kp = 0;
	S->Ki = 0;
	S->Kd = 0;
	arm_pid_init_f32(S,1);	
}
/**
*******************************************************************************************
**函数名：   Cal_IncPID
**函数说明： 增量式PID计算
**形参：     arm_pid_instance_f32 *S: PID控制参数结构体
**           PID PID_ST *Svar       ：PID输入输出参数结构体
**           float SampleValue      : 输入数据
**返回值：   增量式PID输出值
****************************************************************************************
**/
float Cal_IncPID(arm_pid_instance_f32 *S, PID_Typedef *Svar,float SampleValue)
{
	S->state[0] = Svar->SetValue - SampleValue;//误差
	S->state[1] += S->state[0];//误差和
	S->state[2] = S->state[0] - Svar->LastErr;//误差微分
	
	Svar->LastErr = S->state[0];
//	Cal_IncPIDpara(S, Svar);
	
  Svar->Output += S->Kp * S->state[0] + S->Ki * S->state[1] + S->Kd * S->state[2];
	
	Check_IncPIDout(Svar);
	return (uint16_t)Svar->Output;
}
/*
****************************************************************************************
**函数名：  Cal_IncPIDpara
**函数说明：PID控制参数调整
**形参：    arm_pid_instance_f32 *S: PID控制参数结构体
**          PID PID_ST *Svar       ：PID输入输出参数结构体
**返回值：  none
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
**函数名：   Check_IncPIDout
**函数说明： PID输出阈值检测
**形参：     PID_ST *Svar ：PID输入输出参数结构体
**返回值：   none
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
