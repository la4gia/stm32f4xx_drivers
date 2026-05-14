/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Apr 28, 2026
 *      Author: la4gia
 */
#include "stm32f407xx.h"

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_



// Config structure for SPI
typedef struct {
	uint8_t SPI_DeviceMode;			// @SPI_DeviceMode
	uint8_t SPI_BusConfig;			// @SPI_BusConfig
	uint8_t SPI_SclkSpeed;			// @SPI_Clock_Speed
	uint8_t SPI_DFF;				// @SPI_Data_Frame_Format
	uint8_t SPI_CPOL;				// @SPI_Clock_Polarity
	uint8_t SPI_CPHA;				// @SPI_Clock_Phase
	uint8_t SPI_SSM;				// @SPI_Slave_Management
} SPI_Config_t;

// handle structure for SPI
typedef struct {
	SPI_RegDef_t *pSPIx;		// this holds the base address of SPIx(x:1,1,3)
	SPI_Config_t SPIConfig;
} SPI_Handle_t;


// @SPI_DeviceMode options
#define SPI_DEVICE_MODE_SLAVE	0
#define SPI_DEVICE_MODE_MASTER	1

// @SPI_BusConfig options
#define SPI_BUS_CONFIG_FD					1
#define SPI_BUS_CONFIG_HD					2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY		3

// @SPI_Clock_Speed
#define SPI_SCLK_SPEED_DIV2			0
#define SPI_SCLK_SPEED_DIV4			1
#define SPI_SCLK_SPEED_DIV8			2
#define SPI_SCLK_SPEED_DIV16		3
#define SPI_SCLK_SPEED_DIV32		4
#define SPI_SCLK_SPEED_DIV64		5
#define SPI_SCLK_SPEED_DIV128		6
#define SPI_SCLK_SPEED_DIV256		7

// @SPI_Data_Frame_Format
#define SPI_DFF_8BITS		0
#define SPI_DFF_16BITS		1

// @SPI_Clock_Polarity
#define SPI_CPOL_LOW		0
#define SPI_CPOL_HIGH		1

// @SPI_Clock_Phase
#define SPI_CPHA_LOW		0
#define SPI_CPHA_HIGH		1

// @SPI_Slave_Management
#define SPI_SSM_DI			0
#define SPI_SSM_EN			1

// Status Flag definitions
#define SPI_TXE_FLAG		( 1 << SPI_SR_TXE )
#define SPI_RXNE_FLAG		( 1 << SPI_SR_RXNE )
#define SPI_BUSY_FLAG		( 1 << SPI_SR_BSY )

// Prototype APIs Supported by this driver

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

// data APIs
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);

// interrupt APIs
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandle(SPI_Handle_t *pSPIHandle);

// other
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);


#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
