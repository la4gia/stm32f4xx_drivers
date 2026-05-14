/*
 * 006spi_tx_testing.c
 *
 * ALT Function Mode: 5
 * --> NSS
 * PA5 --> SCLK
 * PA6 --> MISO
 * PA7 --> MOSI
 */

#include <stdint.h>
#include <string.h>
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"


void delay() {
	for (volatile uint32_t i = 0; i < 500000/2; i++);
}

void SPI_GPIO_Inits(void) {

	GPIO_Handle_t SPIpins;

	SPIpins.pGPIOx = GPIOA;
	SPIpins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIpins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIpins.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	SPIpins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIpins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	// SCLK
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIO_Init(&SPIpins);

	// MOSI
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&SPIpins);

	// MISO
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&SPIpins);

	// NSS
	// SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	// GPIO_Init(&SPIpins);
}

void SPI_Inits(void) {

	SPI_Handle_t SPIhandle;

	SPIhandle.pSPIx = SPI1;
	SPIhandle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPIhandle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPIhandle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; 		// generates sclk of 2MHz
	SPIhandle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPIhandle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPIhandle.SPIConfig.SPI_CPHA =  SPI_CPHA_LOW;
	SPIhandle.SPIConfig.SPI_SSM = SPI_SSM_EN;						// software slave management enabled

	SPI_Init(&SPIhandle);
}

// void GPIO_Button_Init(void) {
// 	GPIO_Handle_t gpio_button;

// 	gpio_button.pGPIOx = GPIOA;
// 	gpio_button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
// 	gpio_button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
// 	gpio_button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
// 	gpio_button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

// 	GPIO_Init(&gpio_button);
// }


uint8_t SPI_TransferByte(uint8_t data) {

	// wait until TX is empty
	while (!SPI_GetFlagStatus(SPI1, SPI_TXE_FLAG));

	SPI_SendData(SPI1, &data, 1);

	// wait until RX is not empty
 	while (!SPI_GetFlagStatus(SPI1, SPI_RXNE_FLAG));

	uint8_t rx;
	SPI_ReceiveData(SPI1, &rx, 1);

	return rx;
}


int main(void) {

	uint8_t test_data[] = {0xAA, 0X55, 0XF0, 0X0F};

	// configure button
	// GPIO_Button_Init();

	// configure and initialize GPIO pins for SPI
	SPI_GPIO_Inits();

	// initialize SPI peripheral parameters
	SPI_Inits();

	SPI_SSIConfig(SPI1, ENABLE);

	// enable peripheral
	SPI_PeripheralControl(SPI1, ENABLE);
	
	for (int i = 0; i < 4; i++) {
		uint8_t rx = SPI_TransferByte(test_data[i]);
		
		if (rx != test_data[i]) {
			continue;
		}
	}
	// disable peripheral
	SPI_PeripheralControl(SPI1, DISABLE);

	while (1);

	// while (1) {

	// 	// wait until button is pressed
	// 	while (! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		
	// 	// avoid de-bouncing
	// 	delay();
		
	// 	// enable the SPI2 peripheral (peripheral must be disabled when configuring)
	// 	SPI_PeripheralControl(SPI2, ENABLE);
		
	// 	// first, send length of data
	// 	uint8_t data_len = strlen(user_data);
	// 	SPI_SendData(SPI2, &data_len, 0x01);

	// 	// next, send data
	// 	SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));

	// 	// confirm SPI is not busy
	// 	while ( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	// 	// disable SPI2 peripheral
	// 	SPI_PeripheralControl(SPI2, DISABLE);
	// }

	return 0;
}
