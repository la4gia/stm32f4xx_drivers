/*
 * 006spi_tx_testing.c
 *
 * ALT Function Mode: 5
 * PB12 --> NSS
 * PB13 --> SCLK
 * PB14 --> MISO
 * PB15 --> MOSI
 */

#include <stdint.h>
#include <string.h>
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"


void delay() {
	for (volatile uint32_t i = 0; i < 500000/2; i++);
}

void SPI2_GPIO_Inits(void) {

	GPIO_Handle_t SPIpins;

	SPIpins.pGPIOx = GPIOB;
	SPIpins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIpins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIpins.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	SPIpins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIpins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	// SCLK
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIpins);

	// MOSI
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIpins);

	// MISO
	// SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	// GPIO_Init(&SPIpins);

	// NSS
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIpins);
}

void SPI2_Inits(void) {

	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; 		// generates sclk of 2MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA =  SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;						// hardware slave management enabled

	SPI_Init(&SPI2handle);
}

void GPIO_Button_Init(void) {
	GPIO_Handle_t gpio_button;

	gpio_button.pGPIOx = GPIOA;
	gpio_button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	gpio_button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	gpio_button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	gpio_button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&gpio_button);
}

int main(void) {

	char user_data[] = "Hello world";

	// configure button
	GPIO_Button_Init();

	// configure and initialize GPIO pins for SPI2
	SPI2_GPIO_Inits();

	// initialize SPI2 peripheral parameters
	SPI2_Inits();

	/*
	* making SSOE 1 does NSS output. NSS pin automatically managed by the hardware
	* when SPE=1, NSS is pulled to low and vice versa.
	*/
	SPI_SSOEConfig(SPI2, ENABLE);

	while (1) {

		// wait until button is pressed
		while (! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		
		// avoid de-bouncing
		delay();
		
		// enable the SPI2 peripheral (peripheral must be disabled when configuring)
		SPI_PeripheralControl(SPI2, ENABLE);
		
		// first, send length of data
		uint8_t data_len = strlen(user_data);
		SPI_SendData(SPI2, &data_len, 0x01);

		// next, send data
		SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));

		// confirm SPI is not busy
		while ( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		// disable SPI2 peripheral
		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;
}
