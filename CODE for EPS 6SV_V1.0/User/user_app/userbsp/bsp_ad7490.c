/*
*******************************************************************************************************
**bsp_ad7490.c
**
**
********************************************************************************************************
*/

#include "bsp_ad7490.h"

uint16_t ad7490_data[16];
uint16_t shadow_reg = 0xffff;
/*
**AD7490≥ı ºªØ
**
**
*/
void bsp_AD7490_Init(void)
{
	uint32_t i;
	uint16_t ctr_reg;
	//write to control register
  //configure	to shadow mode,normal power mode normal range ,bin coding format
	ctr_reg = WRITE | SEQ_CFG | CHANNEL_0 | POWER_NORMAL | RANGE_NORMAL | DATA_BIN;
	AD_SendByte(ctr_reg);
	for(i=0;i<20000;i++)
	{}
		//write shadow register with all channels selected
	AD_SendByte(ALL_CHANNEL);
		//write to control register with a power off mode
	ctr_reg = WRITE | SEQ_CFG | CHANNEL_0 | POWER_FULLDOWN | RANGE_NORMAL | DATA_BIN;
	AD_SendByte(ctr_reg);
}

uint16_t *ad7490_read_NoIntSeq(void)
{
	uint16_t i;
	uint8_t channel;
	uint16_t *prxdata;
	uint16_t rxdata;
	uint16_t ctr_reg = 0x0000;
	prxdata = &ad7490_data[0];
	//reinit the ad7490
	ctr_reg = WRITE | SEQ_CFG | CHANNEL_0 | POWER_NORMAL | RANGE_NORMAL | DATA_BIN;
	AD_SendByte(ctr_reg);
	for(i=0;i<20000;i++)
	{}
		//write to shadow register
	AD_SendByte(ALL_CHANNEL);
		
	for(i=0;i<16;i++)
	{
		//write 0x0000 to the ad7490 to read converted value
		//the outputs contain the channeel num and adc value in a 16 bits data
		// the format is 4 bits channel num MSB and the 12bits LSB is adc value
		rxdata = AD_SendByte(DATA_UPDATE);		
		channel = (uint8_t)(rxdata>>12);
		ad7490_data[channel] = rxdata & 0x0fff;
//		printf("ad_data[%2d]%4x\r\n",channel,ad7490_data[channel]);
	}
	//configure the ad7490 to power down mode 
	ctr_reg = WRITE | SEQ_CFG | CHANNEL_0 | POWER_FULLDOWN | RANGE_NORMAL | DATA_BIN;
	AD_SendByte(ctr_reg);

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






/****************************************************************************************************/

