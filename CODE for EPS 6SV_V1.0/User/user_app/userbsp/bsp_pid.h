#ifndef __BSP_PID_H__
#define __BSP_PID_H__


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
#include "stm32f4xx.h"
#include "arm_math.h"
	 
typedef struct{
	float SetValue;
  float SumErr;
	float LastErr;
  float ZeroErr;
	float ZeroErrMax;
	float ZeroErrMin;
	float KiMax;
  float KiMin;
  float OutMax;
  float OutMin;
	float Output;
} PID_Typedef;	 
	 
	 
void Init_IncPID(arm_pid_instance_f32 *S);
void Init_IncPIDvar(PID_Typedef *S);

void Cal_IncPIDpara(arm_pid_instance_f32 *S, PID_Typedef *Svar);

void bsp_IncPID_Init(void);
float Cal_IncPID_Plus(arm_pid_instance_f32 *S, PID_Typedef *Svar,float SampleValue);
float Cal_IncPID_Minus(arm_pid_instance_f32 *S, PID_Typedef *Svar,float SampleValue);
void Cal_IncPIDpara(arm_pid_instance_f32 *S, PID_Typedef *Svar);
void Check_IncPIDout(PID_Typedef *Svar);	 
	 
	 
	 
	 
	 
	 
	 
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */	 
	 
#endif

