/*
*********************************************************************************
**bsp_spi.c
**
**
**
**********************************************************************************
*/

#include "bsp_spi.h"

static void spi1_cs1(FunctionalState NewState) {if(NewState == ENABLE) GPIO_ResetBits(GPIOB,GPIO_Pin_6);else GPIO_SetBits(GPIOB,GPIO_Pin_6);}

void bsp_InitSPI(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		SPI_InitTypeDef  SPI_InitStructure;
	SPI_I2S_DeInit(SPI1);
	{
		//SPI1  AD7490    SPI1_SCK--PB3;SPI1_MISO--PB4;SPI1_MOSI--PB5;SPI1_EN1--PB6
		/* ʹ��GPIO ʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		/* ���� SCK, MISO �� MOSI Ϊ���ù��� */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* ����Ƭѡ����Ϊ�������ģʽ */
		GPIO_SetBits(GPIOB,GPIO_Pin_6);		/* Ƭѡ�øߣ���ѡ�� */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}

	{
	/* ��SPI1ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/* ����SPIӲ������ */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* ���ݷ���2��ȫ˫�� */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32��SPI����ģʽ ������ģʽ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;	/* ����λ���� �� 8λ */
	/* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
	   ��������: ���߿����Ǹߵ�ƽ,��2�����أ������ز�������)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			/* ʱ�������ز������� */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		/* ʱ�ӵĵ�2�����ز������� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* Ƭѡ���Ʒ�ʽ��������� */

	/* ���ò�����Ԥ��Ƶϵ�� 
		��SPIʱ�������2��Ƶ����֧�ֲ���Ƶ��
		�����SPI1��2��ƵʱSCKʱ�� = 42M��4��ƵʱSCKʱ�� = 21M
		�����SPI3, 2��ƵʱSCKʱ�� = 21M
	*/
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* ����λ������򣺸�λ�ȴ� */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC����ʽ�Ĵ�������λ��Ϊ7�������̲��� */
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, DISABLE);			/* �Ƚ�ֹSPI  */

	SPI_Cmd(SPI1, ENABLE);				/* ʹ��SPI  */
	}
}
/*
**spi_cs
**para    
**     SPIx   SPI1,SPI2,SPI3
**     chan   ��Ӧѡ��˿�
**     NewState   �˿�״̬
*/    
void spi_cs(SPI_TypeDef* SPIx,uint8_t chan, FunctionalState NewState)
{
	if (SPIx ==SPI1)
	{
		if (chan == 1)
		{
			spi1_cs1(NewState);
		}
	}
	
}


uint16_t spi_trx(SPI_TypeDef* SPIx, uint16_t data)
{
	uint16_t txdata,rxdata;
	uint16_t wtime = 0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
  {
		wtime++;
		if (wtime>20000) return 0;
	}
	wtime = 0;
  SPI_I2S_SendData(SPIx, txdata);
      
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
  {
		wtime++;
		if (wtime>20000) return 0;
	}
	
  rxdata = SPI_I2S_ReceiveData(SPIx);
	return (uint16_t)rxdata;
}









/**************************************************************************************/

