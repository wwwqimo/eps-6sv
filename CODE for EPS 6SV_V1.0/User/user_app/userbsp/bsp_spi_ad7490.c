
#include "bsp_spi_ad7490.h"
#include <stdio.h>

const int AD_Channel[16] =  {CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3,
		                         CHANNEL_4, CHANNEL_5, CHANNEL_6, CHANNEL_7,
		                         CHANNEL_8, CHANNEL_9, CHANNEL_10,CHANNEL_11,
		                         CHANNEL_12,CHANNEL_13,CHANNEL_14,CHANNEL_15};
uint16_t EpsAdValue[16][16];

void bsp_InitSPI1(void)
{
	InitSPI1_GPIO();
	InitSPI1();
}
static void InitSPI1(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_Cmd(SPI1, DISABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BAUD;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
		
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_ERR, ENABLE);
	SPI_Cmd(SPI1, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void InitSPI1_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* ??MSK?MISO?MOSI???? */

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	/* ??MSK?MISO?MOSI???? */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ????? */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	EPS_CS_HIGH();
}

void SPI1_IRQHandler(void)
{
//	OSIntEnter();
//	if(SPI_I2S_GetITStatus(SPI1, SPI_IT_CRCERR) == SET)
//	{
//		SPI_I2S_ClearITPendingBit(SPI1, SPI_IT_CRCERR);
//		EpsTransErr++;
//	}
//	
//	if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_OVR) == SET)
//	{
//		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_OVR);
//		EpsTransErr++;
//	}	
//	
//	if(SPI_I2S_GetITStatus(SPI1, SPI_IT_MODF) == SET)
//	{
//		SPI_I2S_ClearITPendingBit(SPI1, SPI_IT_MODF);
//		EpsTransErr++;
//	}
//	OSIntExit();
}

void EpsDataProcess(uint16_t data)
{
	uint8_t index;
	uint8_t channel;
	uint16_t ad_temp;

	channel = (uint8_t)(data >> 12);  /* ??????? */
	ad_temp = data & 0x0FFF;  /* ??????AD??? */

	if(ad_temp<EPS_AD_HIGH && ad_temp>EPS_AD_LOW)  /* ????AD??????? */
	{
		for(index=0; index<5; index++)
		{
			EpsAdValue[channel][index] = EpsAdValue[channel][index+1];
		}
		EpsAdValue[channel][5] = ad_temp;  /* ?AD???AD?? */
	}
	else  /* AD??? */
	{
		
		for(index=0; index<5; index++)
		{
			EpsAdValue[channel][index] = EpsAdValue[channel][index+1];
		}
		EpsAdValue[channel][5] = ad_temp;  /* ?AD???AD?? */
		
//		EpsAdErr[channel]++;
	}
	printf("channel%2d, %4x\r\n",channel,EpsAdValue[channel][5]);
}

void EPS_Init(void)
{
	uint32_t i;
	uint16_t command;
	command = WRITE | SEQ_CFG | CHANNEL_0 | POWER_NORMAL | RANGE_NORMAL | DATA_BIN;
  
	EPS_SendByte(command);
	for(i=0;i<20000;i++)
	{}
	EPS_SendByte(ALL_CHANNEL);

}

uint8_t EPS_Update(void)
{
	uint16_t data;

	data = EPS_SendByte(DATA_UPDATE);

	EpsDataProcess(data);
	
	return 0;
}

uint16_t EPS_SendByte(uint16_t _ucValue)
{
	uint32_t i=0;
	/* ?????????? */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		if(i++ > 200000)
		{
//			EpsTranOTCnt++;
//			bsp_DelayUS(200);
//			return 1;
		}
	}
	EPS_CS_LOW();
	/* ??SPI1?????? */
	SPI_I2S_SendData(SPI1, _ucValue);

	/* ??????????? */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	if(i++ > 200000)
	{
//		EpsRevOTCnt++;
//		bsp_DelayUS(200);
//		return 1;
	}
	/* ??SPI???? */
	EPS_CS_HIGH();
	return SPI_I2S_ReceiveData(SPI1);
}



