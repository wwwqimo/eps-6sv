/*
*********************************************************************************************************
*
*	模块名称 : ADC驱动模块
*	文件名称 : bsp_adc.c
*	版    本 : V1.0
*	说    明 : ADC多通道扫描
*	修改记录 :
*		版本号    日期        作者     说明
*		V1.0    2014-01-08  armfly   正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp_adc.h"


/* 变量 ----------------------------------------------------------------------*/
__IO uint16_t uhADCConvertedValue[NumofConv];
static void ADC_DMA_Init(void);
static void ADC_GPIO_Init(void);


static void ADC_DMA_Init(void)
{
	DMA_InitTypeDef       DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	/* DMA2 Stream0 channel0 配置-------------------------------------------------- */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADCConvertedValue;;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = BufferSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	/* DMA2_Stream0 enable */
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

static void ADC_GPIO_Init(void)
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	
	/*
	**ADC1 chan 0 chan 1 chan 2 chan 3 chan 4 chan 5 chan 6 chan 7 
	**     PA0    PA1    PA2    PA3    PA4    PA5    PA6    PA7
	**
	**PA4    PA5 亦作DAC1out DAC2out (ADC not used)
	**ADC1 chan 8 chan 9 chan10 chan11 chan12 chan13 chan14 chan15
	**     PB0    PB1    PC0    PC1    PC2    PC3    PC4    PC5
	**
	**USED ADC CAHNS  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 TOTAL 16CHANS
	**
	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitADC
*	功能说明: ADC初始化
*	形    参: 无
*	返 回 值: 无
* ADC通道对应采样数据 
* ADC0--C_S1;ADC1--V_S2;ADC2--C_S2;ADC3--V_S3;ADC4--C_S3;ADC5--C_S4;ADC6--C_S5;
* ADC7--C_S6;ADC8--V_S5;ADC9--V_S4;ADC10--C_BUS;ADC11--V_BUS;ADC12--C_SV;
* ADC13--V_S1;ADC14--V_T2;ADC15--V_S6
*********************************************************************************************************
*/
void bsp_InitADC(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_DeInit();
	/* 使能外设时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
	ADC_GPIO_Init();
  ADC_DMA_Init();
    /****************************************************************************   
	  PCLK2 = HCLK / 2 
	  下面选择的是2分频
	  ADCCLK = PCLK2 /8 = HCLK / 8 = 168 / 8 = 21M
      ADC采样频率： Sampling Time + Conversion Time = 480 + 12 cycles = 492cyc
                    Conversion Time = 21MHz / 492cyc = 42.6ksps. 
	*****************************************************************************/
    
	/* ADC Common 配置 ----------------------------------------------------------*/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 regular channel  configuration ************************************/
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfConversion = NumofConv;
    ADC_Init(ADC1, &ADC_InitStructure);
    
     /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* ADC1 regular channels 0~6 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_144Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_144Cycles);   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_144Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_144Cycles);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_144Cycles);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_144Cycles);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_144Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_144Cycles); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_144Cycles); 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 10, ADC_SampleTime_144Cycles);   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 11, ADC_SampleTime_144Cycles);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 12, ADC_SampleTime_144Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 13, ADC_SampleTime_144Cycles); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 14, ADC_SampleTime_144Cycles); 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 15, ADC_SampleTime_144Cycles);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 16, ADC_SampleTime_144Cycles);
//		/* ADC1 regular channel16 (Vtemp) configuration ******************************/
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 14, ADC_SampleTime_144Cycles);
//    /* ADC1 regular channel18 (VBAT) configuration ******************************/
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_Vbat,15, ADC_SampleTime_144Cycles);
//    /* Enable VBAT channel */
//    ADC_VBATCmd(ENABLE); 
//    
//    ADC_TempSensorVrefintCmd(ENABLE); 

    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    /* Enable ADC1 **************************************************************/
    ADC_Cmd(ADC1, ENABLE);
    
    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConv(ADC1);
}


/*
*************************************************************************************************
  VSENSE：温度传感器的当前输出电压，与ADC_DR 寄存器中的结果ADC_ConvertedValue之间的转换关系为： 
            ADC_ConvertedValue * Vdd
  VSENSE = --------------------------
            Vdd_convert_value(0xFFF)  
    ADC转换结束以后，读取ADC_DR寄存器中的结果，转换温度值计算公式如下：
          V25 - VSENSE
  T(℃) = ------------  + 25
           Avg_Slope
  V25：  温度传感器在25℃时 的输出电压，典型值0.76 V。
  Avg_Slope：温度传感器输出电压和温度的关联参数，典型值2.5 mV/℃。
************************************************************************************************
*/
float GetTemp(uint16_t advalue)
{
    float Vtemp_sensor;
    float  Current_Temp;
    
    Vtemp_sensor = advalue * 3.3f/ 4095;  				           
    Current_Temp = (Vtemp_sensor - 0.76f)/0.0025f + 25;  
    
    return Current_Temp;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
