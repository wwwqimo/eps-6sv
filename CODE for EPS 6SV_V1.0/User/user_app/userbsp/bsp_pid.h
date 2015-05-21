#ifndef __BSP_PID_H__
#define __BSP_PID_H__


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
	 #include "stm32f4xx.h"
	 #include "arm_math.h"
	 
typedef struct PID_ST{
	float SetValue;
	
  float SumErr;
	float LastErr;
  float ZeroErr;
	float ZeroErrMax;
	float ZeroErrMin;
  float Output;
  float KiMax;
  float KiMin;
  float OutMax;
  float OutMin;
} PID_Typedef;	 
	 
	 
	 
void bsp_IncPID_Init(void);
float Cal_IncPID(arm_pid_instance_f32 *S, PID_Typedef *Svar,float SampleValue);
void Cal_IncPIDpara(arm_pid_instance_f32 *S, PID_Typedef *Svar);
void Check_IncPIDout(PID_Typedef *Svar);	 
	 
	 
	 
	 
	 
	 
	 
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */	 
	 
#endif

