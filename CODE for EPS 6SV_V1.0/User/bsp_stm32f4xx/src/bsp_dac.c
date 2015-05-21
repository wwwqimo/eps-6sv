/*
*********************************************************************************************************
*
*	模块名称 : DAC驱动模块
*	文件名称 : bsp_dac.c
*	版    本 : V1.0
*	说    明 : 实现DAC输出三角波
*	修改记录 :
*		版本号    日期        作者     说明
*		V1.0    2013-12-30   armfly   正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"


/*
*********************************************************************************************************
*	函 数 名: bsp_InitDAC
*	功能说明: DAC初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitDAC(void)
{
    //TIM6_Config();
    DAC_Ch1_TriangleConfig();
}

/*
*********************************************************************************************************
*	函 数 名: TIM6_Config
*	功能说明: 配置定时器6，用于触发DAC，每TIM6CLK/256更新一次
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM6_Config(void)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    
    /* 使能TIM6时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    /* --------------------------------------------------------
    system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

    HCLK = SYSCLK / 1     (AHB1Periph)
    PCLK2 = HCLK / 2      (APB2Periph)
    PCLK1 = HCLK / 4      (APB1Periph)

    因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
    因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

    APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
    APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
          
    TIM6 更新周期是 = TIM6CLK / （TIM_Period + 1）/（TIM_Prescaler + 1）= TIM6CLK/256
    ----------------------------------------------------------- */
    
    /* 配置TIM6 */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period = 0xFF;          
    TIM_TimeBaseStructure.TIM_Prescaler = 0;       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    /* TIM6 TRGO 选择 */
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

    /* 使能TIM6 */
    TIM_Cmd(TIM6, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: DAC_Ch1_TriangleConfig
*	功能说明: DAC通道1三角波配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DAC_Ch1_TriangleConfig(void)
{
    DAC_InitTypeDef  DAC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /*  使能引脚时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		/* DAC 外设时钟使能 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

		/* DAC通道1输出引脚配置（PA.4） */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* DAC通道1配置 */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None  ;
    
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    /*使能DAC通道1 */
    DAC_Cmd(DAC_Channel_1, ENABLE);


    /* DAC通道2配置 */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None  ;
    
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /*使能DAC通道2 */
    DAC_Cmd(DAC_Channel_2, ENABLE);
    /* 设置DAC通道1的DHR12RD寄存器 */
    //DAC_SetChannel1Data(DAC_Align_12b_L, 30);
		/* 设置DAC通道2的DHR12RD寄存器 */
    //DAC_SetChannel2Data(DAC_Align_12b_L, 30);
}

/*
*********************************************************************************************************
*	函 数 名: DAC_Ch1_AmplitudeConfig
*	功能说明: DAC通道1三角波幅值配置
*	形    参: _DAC_Amplitude 三角波幅值选项
*	返 回 值: 无
*********************************************************************************************************
*/
void DAC_Ch1_AmplitudeConfig(uint32_t _DAC_Amplitude)
{
    DAC_InitTypeDef  DAC_InitStructure;
    
    /* 禁止DAC通道1 */
    DAC_Cmd(DAC_Channel_1, DISABLE);

    /* DAC通道1配置 */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = _DAC_Amplitude;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    /* 使能DAC通道1 */
    DAC_Cmd(DAC_Channel_1, ENABLE);
}


/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
