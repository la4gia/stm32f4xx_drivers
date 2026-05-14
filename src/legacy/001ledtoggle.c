#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"


void delay() {
	for (volatile uint32_t i = 0; i < 500000/2; i++);
}

int main(void) {

	GPIO_Handle_t gpio_led;

	gpio_led.pGPIOx = GPIOD;
	gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	gpio_led.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	// GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&gpio_led);

	while(1) {
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
		delay();
	}

	return 0;
}
