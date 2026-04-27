#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"


void delay() {
	for (uint32_t i = 0; i < 500000/2; i++);
}

uint8_t verify(GPIO_RegDef_t *pGPIOx, uint8_t pin) {

	uint8_t mode = (pGPIOx->MODER >> (2 * pin)) && 0x3;		// verify output mode
	if (mode != 0x1) return 1;

	uint8_t speed = (pGPIOx->OSPEEDR >> (2 * pin)) && 0x3;	// verify speed
	if (speed != 0x1) return 1;

	uint8_t pupd = (pGPIOx->PUPDR >> (2 * pin)) && 0x3;		// verify pull up pull down
	if (pupd != 0x0) return 1;

	return 0;
}

int main(void) {

	GPIO_Handle_t gpio_led;

	gpio_led.pGPIOx = GPIOD;
	gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	gpio_led.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;


	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&gpio_led);

	uint8_t results = verify(GPIOD, GPIO_PIN_NO_12);
	if (results) {
		return 1;
	}

	while(1) {
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
		delay();
	}

	return 0;
}
