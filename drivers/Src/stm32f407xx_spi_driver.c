#include "stm32f407xx_spi_driver.h"



/*****************************************************************
 * name					- SPI_PeriClockControl
 *
 * scope 				- Enable or disable peripheral clock for the given SPI port
 *
 * param *pGPIOx		- base address of the SPI peripheral
 * param EnOrDi			- ENABLE or DISABLE macros
 *
 * return				- None
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {

	if (EnOrDi == ENABLE) {
		if (pSPIx == SPI1) {
			SPI1_PCLK_EN();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_EN();
		} else if (pSPIx == SPI3) {
			SPI3_PCLK_EN();
		} else if (pSPIx == SPI4) {
			SPI4_PCLK_EN();
		} else if (pSPIx == SPI5) {
			SPI5_PCLK_EN();
		} else if (pSPIx == SPI6) {
			SPI6_PCLK_EN();
		}
	} else {
		if (pSPIx == SPI1) {
			SPI1_PCLK_DI();
		} else if (pSPIx == SPI2) {
			SPI2_PCLK_DI();
		} else if (pSPIx == SPI3) {
			SPI3_PCLK_DI();
		} else if (pSPIx == SPI4) {
			SPI4_PCLK_DI();
		} else if (pSPIx == SPI5) {
			SPI5_PCLK_DI();
		} else if (pSPIx == SPI6) {
			SPI6_PCLK_DI();
		}
	}
}

/*****************************************************************
 * name                 - SPI_Init
 *
 * scope                - Initialize and configure SPI based on user settings
 *
 * param *pSPIHandle   - pointer to SPI handle structure containing
 *                        configuration and base address
 *
 * return               - None
 */
void SPI_Init(SPI_Handle_t *pSPIHandle) {


	uint32_t temp = 0;

	// enable peripheral clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	// configure device mode
	temp |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	// configure bus config
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
		// clear Bidirectional data mode
		temp &= ~(1 << 15);
	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
		// set Bidirectional data mode
		temp |= (1 << 15);
	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY) {
		// clear Bidirectional data mode
		temp  &= ~(1 << 15);
		// set RX mode
		temp |= (1 << 10);
	}

	// clock speed
	temp |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << 3);

	// data frame format
	temp |= (pSPIHandle->SPIConfig.SPI_DFF << 11);

	// set clock polarity
	temp |= (pSPIHandle->SPIConfig.SPI_CPOL << 1);

	// set clock phase
	temp |= (pSPIHandle->SPIConfig.SPI_CPHA << 0);

	// set slave management
	temp |= (pSPIHandle->SPIConfig.SPI_SSM << 9);

	pSPIHandle->pSPIx->SPI_CR1 = temp;
}


/*****************************************************************
 * name                 - SPI_DeInit
 *
 * scope                - Reset the SPI peripheral registers to default values
 *
 * param *pSPIx        - base address of the SPI peripheral
 *
 * return               - None
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx) {

	if (pSPIx == SPI1) {
		SPI1_REG_RESET();
	} else if (pSPIx == SPI2) {
		SPI2_REG_RESET();
	} else if (pSPIx == SPI3) {
		SPI3_REG_RESET();
	} else if (pSPIx == SPI4) {
		SPI4_REG_RESET();
	} else if (pSPIx == SPI5) {
		SPI5_REG_RESET();
	} else if (pSPIx == SPI6) {
		SPI6_REG_RESET();
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName) {

	if (pSPIx->SPI_SR & FlagName) {
		return FLAG_SET;
	}
	return FLAG_RESET;
}


/*****************************************************************
 * name                 - SPI_SendData
 *
 * scope                - Send data using TX of SPI. The "blocking" API.
 *
 * param *pSPIx        - base address of the SPI peripheral
 * param *pTxBuffer	   - base address of the TX buffer (the data)
 * param len		   - length of the TX buffer
 *
 * return               - None
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len) {

	while (len > 0) {
		// 1. wait until TXE is set (TXE = Tx Empty)
		while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == (uint8_t)FLAG_RESET);

		// 2. check the DFF bit in CR1
		if ( pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF ) ) {
			// 16 BIT DFF
			// 1. load the data into the DR
			pSPIx->SPI_DR = *((uint16_t*)pTxBuffer);
			len--;
			len--;
			(uint16_t*)pTxBuffer++;
		} else {
			// 8 bit DFF
			pSPIx->SPI_DR = *pTxBuffer;
			len--;
			pTxBuffer++;
		}
	}
}



void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);



/*****************************************************************
 * name                 - SPI_PeripheralControl
 *
 * scope                - Enable or disable peripheral
 *
 * param *pSPIx        - base address of the SPI peripheral
 * param EnOrDi			- ENABLE or DISABLE
 *
 * return               - None
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {

	if (EnOrDi == ENABLE) {
		pSPIx->SPI_CR1 |= (1 << SPI_CR1_SPE);
	} else {
		pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

/*****************************************************************
 * name                 - SPI_SSIConfig
 *
 * scope                - Enable or disable peripheral
 *
 * param *pSPIx        - base address of the SPI peripheral
 * param EnOrDi			- ENABLE or DISABLE
 *
 * return               - None
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {

	if (EnOrDi == ENABLE) {
		pSPIx->SPI_CR1 |= (1 << SPI_CR1_SSI);
	} else {
		pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SSI);
	}
}
