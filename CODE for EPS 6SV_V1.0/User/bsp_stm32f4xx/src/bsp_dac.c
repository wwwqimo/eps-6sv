/*
*********************************************************************************************************
*
*	ģ������ : DAC����ģ��
*	�ļ����� : bsp_dac.c
*	��    �� : V1.0
*	˵    �� : ʵ��DAC������ǲ�
*	�޸ļ�¼ :
*		�汾��    ����        ����     ˵��
*		V1.0    2013-12-30   armfly   ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitDAC
*	����˵��: DAC��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitDAC(void)
{
    //TIM6_Config();
    DAC_Ch1_TriangleConfig();
}

/*
*********************************************************************************************************
*	�� �� ��: TIM6_Config
*	����˵��: ���ö�ʱ��6�����ڴ���DAC��ÿTIM6CLK/256����һ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM6_Config(void)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    
    /* ʹ��TIM6ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    /* --------------------------------------------------------
    system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

    HCLK = SYSCLK / 1     (AHB1Periph)
    PCLK2 = HCLK / 2      (APB2Periph)
    PCLK1 = HCLK / 4      (APB1Periph)

    ��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
    ��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

    APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
    APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
          
    TIM6 ���������� = TIM6CLK / ��TIM_Period + 1��/��TIM_Prescaler + 1��= TIM6CLK/256
    ----------------------------------------------------------- */
    
    /* ����TIM6 */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period = 0xFF;          
    TIM_TimeBaseStructure.TIM_Prescaler = 0;       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    /* TIM6 TRGO ѡ�� */
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

    /* ʹ��TIM6 */
    TIM_Cmd(TIM6, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: DAC_Ch1_TriangleConfig
*	����˵��: DACͨ��1���ǲ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DAC_Ch1_TriangleConfig(void)
{
    DAC_InitTypeDef  DAC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /*  ʹ������ʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		/* DAC ����ʱ��ʹ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

		/* DACͨ��1����������ã�PA.4�� */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* DACͨ��1���� */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None  ;
    
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    /*ʹ��DACͨ��1 */
    DAC_Cmd(DAC_Channel_1, ENABLE);


    /* DACͨ��2���� */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None  ;
    
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /*ʹ��DACͨ��2 */
    DAC_Cmd(DAC_Channel_2, ENABLE);
    /* ����DACͨ��1��DHR12RD�Ĵ��� */
    //DAC_SetChannel1Data(DAC_Align_12b_L, 30);
		/* ����DACͨ��2��DHR12RD�Ĵ��� */
    //DAC_SetChannel2Data(DAC_Align_12b_L, 30);
}

/*
*********************************************************************************************************
*	�� �� ��: DAC_Ch1_AmplitudeConfig
*	����˵��: DACͨ��1���ǲ���ֵ����
*	��    ��: _DAC_Amplitude ���ǲ���ֵѡ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DAC_Ch1_AmplitudeConfig(uint32_t _DAC_Amplitude)
{
    DAC_InitTypeDef  DAC_InitStructure;
    
    /* ��ֹDACͨ��1 */
    DAC_Cmd(DAC_Channel_1, DISABLE);

    /* DACͨ��1���� */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = _DAC_Amplitude;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    /* ʹ��DACͨ��1 */
    DAC_Cmd(DAC_Channel_1, ENABLE);
}


/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
