/*
*******************************************************************************************************
**bsp_ad7490.c
**
**
********************************************************************************************************
*/

#include "bsp_ad7490.h"

static uint16_t ad7490_DeInit(void);
static void ad7490_cs(FunctionalState NewState){ spi_cs(SPI1,1,NewState);}

uint16_t ad7490_data[16];
uint16_t shadow_reg = 0xffff;
/*
**AD7490初始化
**
**
*/
void bsp_AD7490_Init(void)
{
	uint32_t i;
	uint16_t command;
	
	command = WRITE | SEQ_CFG | CHANNEL_0 | POWER_NORMAL | RANGE_NORMAL | DATA_BIN;
	AD_SendByte(command);
	for(i=0;i<20000;i++)
	{}
	AD_SendByte(ALL_CHANNEL);
}

uint16_t *ad7490_read_NoIntSeq(void)
{
	uint16_t i;
	uint8_t channel;
	uint16_t *prxdata;
	uint16_t rxdata;
	uint16_t ctr_reg = 0x0000;
	prxdata = &ad7490_data[0];

	for(i=0;i<16;i++)
	{
		rxdata = AD_SendByte(DATA_UPDATE);		
		channel = (uint8_t)(rxdata>>12);
		ad7490_data[channel] = rxdata & 0x0fff;
//		printf("ad_data[%2d]%4x\r\n",channel,ad7490_data[channel]);
	}
	

	return prxdata;
}

uint16_t AD_SendByte(uint16_t _ucValue)
{
	uint32_t i=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		if(i++ > 200000)
		{
		}
	}
	AD_CS_LOW();
	SPI_I2S_SendData(SPI1, _ucValue);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	if(i++ > 200000)
	{
	}
	AD_CS_HIGH();
	return SPI_I2S_ReceiveData(SPI1);
}
/*
**
**power_mode:
**		FullShutdown 模式则ad7490停止工作；
**		AutoShutdown 和AutoStandby 模式只进行单词转换，转换结束后进入睡眠，需要一个虚拟周期唤醒
**		（一次完整的CS和16clk）
**		NormalOperation  全速正常工作，耗电量最大
**seq_mode:
**		NoSeq         只进行一次转换，转换通道为写入的地址通道，转换数据下次读取有效
**    ShadowSeq     shadow 模式，shadow位写入1，下一次写入的应该为sahdow寄存器，之后进行连续转换
**    NoIntSeq      连续转换模式
**    SelectSeq     根据控制寄存器中ADD3--ADD0确定的地址，从chan0 到选定的通道队列进行转换
*/











/****************************************************************************************************/

