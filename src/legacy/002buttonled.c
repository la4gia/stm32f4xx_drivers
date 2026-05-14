#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"


void delay() {
	for (uint32_t i = 0; i < 500000/2; i++);
}

int main(void) {

//	GPIO_Handle_t gpio_led;
//
//	gpio_led.pGPIOx = GPIOD;
//	gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
//	gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
//	gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
//	gpio_led.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
//	gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//
//	GPIO_PeriClockControl(GPIOD, ENABLE);
//	GPIO_Init(&gpio_led);

	GPIO_Handle_t gpio_but;

	gpio_but.pGPIOx = GPIOA;
	gpio_but.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	gpio_but.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	gpio_but.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	gpio_but.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&gpio_but);

	while(1) {
		if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)) {
			delay();
			GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
		}
	}

	return 0;
}
