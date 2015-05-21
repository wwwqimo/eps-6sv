
#include "bsp.h"
/*
	可以输出到GPIO的TIM通道:

	TIM1_CH1, PA8,	PE9,
	TIM1_CH2, PA9,	PE11
	TIM1_CH3, PA10,	PE13
	TIM1_CH4, PA11,	PE14

	TIM2_CH1,
	TIM2_CH2, PA1,	PB3
	TIM2_CH3, PA2,	PB10
	TIM2_CH4, PA3,	PB11

	TIM3_CH1, PA6,  PB4, PC6
	TIM3_CH2, PA7,	PB5, PC7
	TIM3_CH3, PB0,	PC8
	TIM3_CH4, PB1,	PC9

	TIM4_CH1, PB6,  PD12
	TIM4_CH2, PB7,	PD13
	TIM4_CH3, PB8,	PD14
	TIM4_CH4, PB9,	PD15

	TIM5_CH1, PA0,  PH10
	TIM5_CH2, PA1,	PH11
	TIM5_CH3, PA2,	PH12
	TIM5_CH4, PA3,	PI10

	TIM8_CH1, PC6,  PI5
	TIM8_CH2, PC7,	PI6
	TIM8_CH3, PC8,	PI7
	TIM8_CH4, PC9,	PI2

	TIM9_CH1, PA2,  PE5
	TIM9_CH2, PA3,	PE6

	TIM10_CH1, PB8,  PF6

	TIM11_CH1, PB9,  PF7

	TIM12_CH1, PB14,  PH6
	TIM12_CH2, PB15,  PH9

	TIM13_CH1, PA6,  PF8
	TIM14_CH1, PA7,  PF9

	APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
	APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
*/


/*-------------------------------------------------------------------------------------------------*/
#define TIM_PWM1 1
#define TIM_PWM2 2
#define TIM_PWM3 3
#define TIM_PWM4 4
/*--初始PWM输出占空比为0-----------------------------------------------------------------------------------------------*/
uint16_t TIM1_CCR_Val[4] = {0};//
uint16_t TIM4_CCR_Val[4] = {0};//


uint16_t PrescalerValue = 0;
/*--函数声明--------------------------------------------------------------------------------------------*/
static void TIM4_PWM_Config(void);
static void TIM1_PWM_Config(void);
static void TIM_GPIO_Config(void);

void bsp_InitPWM(void)
{
	
 /* TIM Configuration */
  TIM_GPIO_Config();
	TIM1_PWM_Config();
  TIM4_PWM_Config();
  
}
/*
****************************************************************************
*函数名：  PWM_Update
*函数说明：PWM占空比更新
*形参：    none
*返回值：  none
****************************************************************************
*/
void PWM_Updata(TIM_TypeDef * TIMx, uint16_t PWM1CCRVal,uint16_t PWM2CCRVal,uint16_t PWM3CCRVal,uint16_t PWM4CCRVal)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PWM2CCRVal;
  TIM_OC2Init(TIMx, &TIM_OCInitStructure);
//	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
	
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PWM3CCRVal;
  TIM_OC3Init(TIMx, &TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
	
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = PWM4CCRVal;
  TIM_OC4Init(TIMx, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);


	TIM_OCInitStructure.TIM_Pulse = PWM1CCRVal;
  TIM_OC1Init(TIMx, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	
//	TIM_ARRPreloadConfig(TIMx, ENABLE);
//	TIM_Cmd(TIMx, ENABLE);
}

/*
**************************************************************************************************
*函数名：  TIM1_PWM_Config
*函数说明：TIM1 PWM 复用功能配置
*形参：    none
*返回值：  none
*******************************************************************************************************
*/
static void TIM1_PWM_Config(void)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;
//	/* -----------------------------------------------------------------------
//    TIM1 Configuration: generate 4 PWM signals with 4 different duty cycles.
//    
//    In this example TIM1 input clock (TIM1CLK) is set to 2 * APB1 clock (PCLK1), 
//    since APB1 prescaler is different from 1.   
//      TIM1CLK = 2 * PCLK1  
//      PCLK1 = HCLK / 4 
//      => TIM1CLK = HCLK / 2 = SystemCoreClock /2
//          
//    To get TIM1 counter clock at 64 MHz, the prescaler is computed as follows:
//       Prescaler = (TIM1CLK / TIM1 counter clock) - 1
//       Prescaler = ((SystemCoreClock /2) /64 MHz) - 1
//                                              
//    To get TIM1 output clock at 82 KHz, the period (ARR)) is computed as follows:
//       ARR = (TIM1 counter clock / TIM1 output clock) - 1
//           = 1023
//                  
//    TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 = 50%
//    TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR)* 100 = 37.5%
//    TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR)* 100 = 25%
//    TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR)* 100 = 12.5%

//    Note: 
//     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
//     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
//     function to update SystemCoreClock variable value. Otherwise, any configuration
//     based on this variable will be incorrect.    
//  ----------------------------------------------------------------------- */  

//  /* Compute the prescaler value */
//	//82kHz

////////////////////////////////////////////////////////////////////////////////////////////////////

  /* -----------------------------------------------------------------------
   
  ----------------------------------------------------------------------- */  

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock) / 64000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1023;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死去控制
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//????,TIM_OCNPolarity_High?????
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  

	TIM_OCInitStructure.TIM_Pulse = TIM1_CCR_Val[1];
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//CCR???????????
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = TIM1_CCR_Val[2];
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//CCR???????????
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = TIM1_CCR_Val[3];
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//CCR???????????
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
  
	TIM_OCInitStructure.TIM_Pulse = TIM1_CCR_Val[0];
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//CCR???????????
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	
  TIM_ARRPreloadConfig(TIM1, ENABLE); //ARR??????????,?????

	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}
/*
**************************************************************************************************
*函数名：  TIM4_PWM_Config
*函数说明：TIM4 PWM 复用功能配置
*形参：    none
*返回值：  none
*******************************************************************************************************
*/
static void TIM4_PWM_Config(void)
{
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;
//	/* -----------------------------------------------------------------------
//    TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles.
//    
//    In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
//    since APB1 prescaler is different from 1.   
//      TIM4CLK = 2 * PCLK1  
//      PCLK1 = HCLK / 4 
//      => TIM4CLK = HCLK / 2 = SystemCoreClock /2
//          
//    To get TIM4 counter clock at 64 MHz, the prescaler is computed as follows:
//       Prescaler = (TIM4CLK / TIM4 counter clock) - 1
//       Prescaler = ((SystemCoreClock /2) /64 MHz) - 1
//                                              
//    To get TIM4 output clock at 82 KHz, the period (ARR)) is computed as follows:
//       ARR = (TIM4 counter clock / TIM4 output clock) - 1
//           = 1023
//                  
//    TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
//    TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 37.5%
//    TIM4 Channel3 duty cycle = (TIM4_CCR3/ TIM4_ARR)* 100 = 25%
//    TIM4 Channel4 duty cycle = (TIM4_CCR4/ TIM4_ARR)* 100 = 12.5%

//    Note: 
//     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
//     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
//     function to update SystemCoreClock variable value. Otherwise, any configuration
//     based on this variable will be incorrect.    
//  ----------------------------------------------------------------------- */  

//  /* Compute the prescaler value */
//	//82kHz

////////////////////////////////////////////////////////////////////////////////////////////////////

  /* -----------------------------------------------------------------------
   
  ----------------------------------------------------------------------- */  

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 64000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1023;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = TIM4_CCR_Val[0];
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = TIM4_CCR_Val[1];
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

//  /* PWM1 Mode configuration: Channel3 */
////  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = TIM4_CCR_Val[2];
//  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
//  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

//  /* PWM1 Mode configuration: Channel4 */
////  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = TIM4_CCR_Val[3];
//  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
//  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM4, ENABLE);

  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
}


/**
  * @brief  Configure the TIM4 Ouput Channels.
  * @param  None
  * @retval None
  */
static void TIM_GPIO_Config(void)
{
	//TIM1 CH1-PE9;CH2-PE11;CH3-PE13;CH4-PE14
	//TIM4 CH1-PD12;CH2-PD13
  GPIO_InitTypeDef GPIO_InitStructure;
	
/*-TIM4 GPIO Config-----------------------------------------------------------------------------------------------*/
  
/* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  /* GPIOD Configuration: TIM4 CH1 (PD12) and TIM4 CH2 (PD13),TIM4 CH3 (PD14),TIM4 CH4 (PD15) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

  /* Connect TIM4 pins to AF2 */  
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); 

	
/*-TIM1 GPIO Config-----------------------------------------------------------------------------------------------*/

  /* GPIOE clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
 	  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  /* GPIOE Configuration: TIM1 CH1 (PE9) and TIM1 CH2 (PE11),TIM1 CH3 (PE13),TIM1 CH4 (PE14) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 

  /* Connect TIM1 pins to AF2 */  
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1); 
}





















/***********END OF FILE***********************************************************************************************************/
