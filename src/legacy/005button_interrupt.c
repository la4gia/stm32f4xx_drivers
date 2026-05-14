#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include <string.h>

#define HIGH 1
#define LOW 0
#define BTN_PRESSED LOW


void delay() {
	for (uint32_t i = 0; i < 500000/2; i++);
}

int main(void) {

	GPIO_Handle_t gpio_led;
	memset(&gpio_led, 0, sizeof(gpio_led));

	gpio_led.pGPIOx = GPIOD;
	gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
	gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	gpio_led.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&gpio_led);

	GPIO_Handle_t gpio_but;
	memset(&gpio_but, 0, sizeof(gpio_but));

	gpio_but.pGPIOx = GPIOD;
	gpio_but.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	gpio_but.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	gpio_but.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	gpio_but.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&gpio_but);

	GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_NO_11, GPIO_PIN_RESET);
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI5_9, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI5_9, ENABLE);

	while(1);

	return 0;
}

// ISR
void EXTI9_5_IRQHandler(void) {
	delay();
	GPIO_IRQHandle(GPIO_PIN_NO_9);		// CLEAR THE PENDING EVENT FROM EXTI LINE
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_11);
}
