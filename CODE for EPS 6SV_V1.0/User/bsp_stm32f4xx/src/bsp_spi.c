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
		/* 使能GPIO 时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		/* 配置 SCK, MISO 、 MOSI 为复用功能 */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* 配置片选口线为推挽输出模式 */
		GPIO_SetBits(GPIOB,GPIO_Pin_6);		/* 片选置高，不选中 */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}

	{
	/* 打开SPI1时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;	/* 数据位长度 ： 8位 */
	/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   本例配置: 总线空闲是高电平,第2个边沿（上升沿采样数据)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			/* 时钟上升沿采样数据 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		/* 时钟的第2个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* 片选控制方式：软件控制 */

	/* 设置波特率预分频系数 
		【SPI时钟最快是2分频，不支持不分频】
		如果是SPI1，2分频时SCK时钟 = 42M，4分频时SCK时钟 = 21M
		如果是SPI3, 2分频时SCK时钟 = 21M
	*/
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC多项式寄存器，复位后为7。本例程不用 */
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, DISABLE);			/* 先禁止SPI  */

	SPI_Cmd(SPI1, ENABLE);				/* 使能SPI  */
	}
}
/*
**spi_cs
**para    
**     SPIx   SPI1,SPI2,SPI3
**     chan   对应选择端口
**     NewState   端口状态
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

