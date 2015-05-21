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
**AD7490��ʼ��
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
**		FullShutdown ģʽ��ad7490ֹͣ������
**		AutoShutdown ��AutoStandby ģʽֻ���е���ת����ת�����������˯�ߣ���Ҫһ���������ڻ���
**		��һ��������CS��16clk��
**		NormalOperation  ȫ�������������ĵ������
**seq_mode:
**		NoSeq         ֻ����һ��ת����ת��ͨ��Ϊд��ĵ�ַͨ����ת�������´ζ�ȡ��Ч
**    ShadowSeq     shadow ģʽ��shadowλд��1����һ��д���Ӧ��Ϊsahdow�Ĵ�����֮���������ת��
**    NoIntSeq      ����ת��ģʽ
**    SelectSeq     ���ݿ��ƼĴ�����ADD3--ADD0ȷ���ĵ�ַ����chan0 ��ѡ����ͨ�����н���ת��
*/











/****************************************************************************************************/

