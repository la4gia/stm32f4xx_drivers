#include "stm32f407xx.h"

typedef struct {

	uint8_t GPIO_PinNumber;
	uint8_t	GPIO_PinMode;				// possible values: @GPIO_PIN_MODES
	uint8_t GPIO_PinOPtype;				// possible values: @GPIO_OUTPUT_TYPES
	uint8_t GPIO_PinSpeed;				// possible values: @GPIO_SPEEDS
	uint8_t GPIO_PinPuPdControl;		// possible values: @GPIO_PULL-UP_PULL-DOWN
	uint8_t GPIO_PinAltFunMode;

} GPIO_PinConfig_t;

typedef struct {

	GPIO_RegDef_t *pGPIOx;				// pointer to the GPIO base addresses of the port to the pin
	GPIO_PinConfig_t GPIO_PinConfig;	// GPIO pin configurations

} GPIO_Handle_t;

// @GPIO_PIN_NUMBERS
#define GPIO_PIN_NO_0		0
#define GPIO_PIN_NO_1   	1
#define GPIO_PIN_NO_2   	2
#define GPIO_PIN_NO_3   	3
#define GPIO_PIN_NO_4   	4
#define GPIO_PIN_NO_5   	5
#define GPIO_PIN_NO_6   	6
#define GPIO_PIN_NO_7   	7
#define GPIO_PIN_NO_8   	8
#define GPIO_PIN_NO_9   	9
#define GPIO_PIN_NO_10		10
#define GPIO_PIN_NO_11		11
#define GPIO_PIN_NO_12		12
#define GPIO_PIN_NO_13		13
#define GPIO_PIN_NO_14		14
#define GPIO_PIN_NO_15		15

// @GPIO_PIN_MODES
#define GPIO_MODE_IN		0
#define GPIO_MODE_OUT		1
#define GPIO_MODE_ALTFN		2
#define GPIO_MODE_ANALOG	3
#define GPIO_MODE_IT_FT		4
#define GPIO_MODE_IT_RT		5
#define GPIO_MODE_IT_RFT	6

// @GPIO_OUTPUT_TYPES
#define GPIO_OP_TYPE_PP		0
#define GPIO_OP_TYPE_OD		1

// @GPIO_SPEEDS
#define GPIO_SPEED_LOW		0
#define GPIO_SPEED_MED		1
#define GPIO_SPEED_HIGH		2
#define GPIO_SPEED_FAST		3

// @GPIO_PULL-UP_PULL-DOWN
#define GPIO_NO_PUPD		0
#define GPIO_PIN_PU			1
#define GPIO_PIN_PD			2

// Prototype APIs Supported by this driver

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteTOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnOrDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandle(uint8_t PinNumber);
