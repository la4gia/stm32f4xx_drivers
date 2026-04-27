#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

#define HIGH 1
#define LOW 0
#define BTN_PRESSED LOW


int main(void) {

	GPIO_Handle_t gpio_led;

	gpio_led.pGPIOx = GPIOA;
	gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	gpio_led.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&gpio_led);

	while(1) {
		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_8);
	}

	return 0;
}
