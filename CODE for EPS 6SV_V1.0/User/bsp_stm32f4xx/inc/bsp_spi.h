/*
*************************************************************************************
**bsp_spi.h
**
**
**
***************************************************************************************
*/

#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__


#include "stm32f4xx.h"



void bsp_InitSPI(void);
void spi_cs(SPI_TypeDef* SPIx,uint8_t chan, FunctionalState NewState);
uint16_t spi_trx(SPI_TypeDef* SPIx, uint16_t data);
#endif

/*******************************************************************************************/

