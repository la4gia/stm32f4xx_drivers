#include "stm32f407xx_gpio_driver.h"


/*****************************************************************
 * name					- GPIO_PeriClockControl
 *
 * scope 				- Enable or disable peripheral clock for the given GPIO port
 *
 * param *pGPIOx		- base address of the GPIO peripheral
 * param EnOrDi			- ENABLE or DISABLE macros
 *
 * return				- None
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi) {
	if (EnOrDi == ENABLE) {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_EN();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_EN();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_EN();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_EN();
		}
	} else {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_DI();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_DI();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_DI();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_DI();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_DI();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_DI();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_DI();
		}
	}
}


/*****************************************************************
 * name                 - GPIO_Init
 *
 * scope                - Initialize and configure a GPIO pin based on user settings
 *
 * param *pGPIOHandle   - pointer to GPIO handle structure containing
 *                        pin configuration and port base address
 *
 * return               - None
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle) {

	uint32_t tmp = 0;

//	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	// configure mode
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG) {

		tmp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); // clearing
		pGPIOHandle->pGPIOx->MODER |= tmp;

	} else {
		// interrupt mode logic
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT) {
			// 1. configure FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// clear the corresponding
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT) {
			// 1. configure RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// clear the corresponding
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT) {
			// 1. configure both FTSR and FTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		// 2. configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t tmp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t tmp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[tmp1] = portcode << (tmp2 * 4);

		// 3. enable the EXTI interrupt deliver using IMR
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	tmp = 0;


	// configure output type
	tmp = pGPIOHandle->GPIO_PinConfig.GPIO_PinOPtype << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->OTYPER |= tmp;
	tmp = 0;

	// configure speed
	tmp = pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); // clearing
	pGPIOHandle->pGPIOx->OSPEEDR |= tmp;
	tmp = 0;


	// configure pull-up pull-down
	tmp = pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); // clearing
	pGPIOHandle->pGPIOx->PUPDR |= tmp;

	// configure alternate function
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode) {

		uint8_t tmp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		uint8_t tmp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[tmp1] &= ~(0xFF << (4 * tmp2));	// clearing
		pGPIOHandle->pGPIOx->AFR[tmp1] |= pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	}

}


/*****************************************************************
 * name                 - GPIO_DeInit
 *
 * scope                - Reset the GPIO peripheral registers to default values
 *
 * param *pGPIOx        - base address of the GPIO peripheral
 *
 * return               - None
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx) {

	if (pGPIOx == GPIOA) {
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB) {
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC) {
		GPIOC_REG_RESET();
	} else if (pGPIOx == GPIOD) {
		GPIOD_REG_RESET();
	} else if (pGPIOx == GPIOE) {
		GPIOE_REG_RESET();
	} else if (pGPIOx == GPIOF) {
		GPIOF_REG_RESET();
	} else if (pGPIOx == GPIOG) {
		GPIOG_REG_RESET();
	} else if (pGPIOx == GPIOH) {
		GPIOH_REG_RESET();
	} else if (pGPIOx == GPIOI) {
		GPIOI_REG_RESET();
	}
}


/*****************************************************************
 * name                 - GPIO_ReadFromInputPin
 *
 * scope                - Read the value from a specific GPIO input pin
 *
 * param *pGPIOx        - base address of the GPIO peripheral
 * param PinNumber      - GPIO pin number to read from
 *
 * return               - value of the pin (0 or 1)
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {

	return (uint8_t)((pGPIOx->IDR  >> PinNumber) & 0x00000001);
}


/*****************************************************************
 * name                 - GPIO_ReadFromInputPort
 *
 * scope                - Read the entire 16-bit value of the GPIO input port
 *
 * param *pGPIOx        - base address of the GPIO peripheral
 *
 * return               - 16-bit value representing all pin states
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx) {

	return (uint16_t)pGPIOx->IDR;
}


/*****************************************************************
 * name                 - GPIO_WriteToOutputPin
 *
 * scope                - Write a value to a specific GPIO output pin
 *
 * param *pGPIOx        - base address of the GPIO peripheral
 * param PinNumber      - GPIO pin number to write to
 * param value          - value to write (SET or RESET)
 *
 * return               - None
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value) {

	if (value == GPIO_PIN_SET ) {
		pGPIOx->ODR |= (1 << PinNumber);
	} else {
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

/*****************************************************************
 * name                 - GPIO_WriteToOutputPort
 *
 * scope                - Write a 16-bit value to the entire GPIO output port
 *
 * param *pGPIOx        - base address of the GPIO peripheral
 * param value          - 16-bit value to write to the port
 *
 * return               - None
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value) {

	pGPIOx->ODR = value;
}


/*****************************************************************
 * name                 - GPIO_ToggleOutputPin
 *
 * scope                - Toggle the current state of a specific GPIO output pin
 *
 * param *pGPIOx        - base address of the GPIO peripheral
 * param PinNumber      - GPIO pin number to toggle
 *
 * return               - None
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {

	pGPIOx->ODR ^= ( 1 << PinNumber);
}


/*****************************************************************
 * name                 - GPIO_IRQInterruptConfig
 *
 * scope                - Configure IRQ number, priority, and enable/disable state
 *
 * param IRQNumber      - IRQ number associated with the GPIO pin
 * param IRQPriority    - priority level of the interrupt
 * param EnOrDi         - ENABLE or DISABLE macros
 *
 * return               - None
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi) {

	if (EnOrDi) {
		if (IRQNumber <= 31) {
			// program ISER0 register
			*NVIC_ISER0 |= ( 1 << IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber <= 63) {
			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32));
		} else if (IRQNumber > 63 && IRQNumber <= 95) {
			*NVIC_ISER2 |= ( 1 << (IRQNumber % 64));
		}
	} else {
		if (IRQNumber <= 31) {
			// program ICER0 register
			*NVIC_ICER0 |= ( 1 << IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber <= 63) {
			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32));
		} else if (IRQNumber > 63 && IRQNumber <= 95) {
			*NVIC_ICER2 |= ( 1 << (IRQNumber % 64));
		}
	}
}

/*****************************************************************
 * name                 - GPIO_IRQPriorityConfig
 *
 * scope                - Configure IRQ number, priority, and enable/disable state
 *
 * param IRQPriority    - priority level of the interrupt
 *
 * return               - None
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {

	// find out the IPR register
	uint8_t iprx = IRQNumber / 4;				// get register
	uint8_t iprx_section = IRQNumber % 4;		// get section in register


	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) |= ( IRQPriority << shift_amount );
}


/*****************************************************************
 * name                 - GPIO_IRQHandle
 *
 * scope                - Handle interrupt for a given GPIO pin (clears interrupt and executes logic)
 *
 * param PinNumber      - GPIO pin number that triggered the interrupt
 *
 * return               - None
 */
void GPIO_IRQHandle(uint8_t PinNumber) {

	// clear the EXTI PR register
	if (EXTI->PR & ( 1 << PinNumber)) {
		// clear (manual says to set as 1)
		EXTI->PR |= ( 1 << PinNumber );
	}
}


/*****************************************************************
 * name                 - GPIO_LockPin
 *
 * scope                - Lock the Pin configuration for GPIO Register
 *
 * param *pGPIOx        - base address of the GPIO peripheral
 * param PinNumber      - GPIO pin number that will be locked
 *
 * return               - None
 */
void GPIO_LockPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){

	uint32_t temp = (1 << PinNumber);

	// step 1: write with 1
	pGPIOx->LCKR = (1 << 16) | temp;

	// step 2: write with 0
	pGPIOx->LCKR = temp;

	// step 3: write with 1
	pGPIOx->LCKR = (1 << 16) | temp;

	// step 4: dummy read
	(void)pGPIOx->LCKR;
}

