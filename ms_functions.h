#ifndef __MS_FUNCTIONS__
#define __MS_FUNCTIONS__

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"

// Led enum shorthand
typedef enum {
	LED_ORANGE = GPIO_PIN_13,
	LED_GREEN = GPIO_PIN_12,
	LED_RED = GPIO_PIN_14,
	LED_BLUE = GPIO_PIN_15,
	LED_ALL = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15
} led_t;

void msGPIO_Init(void);
void msTIM_Init(void);
void LedOff(led_t x);
void LedOn(led_t x);
void delay(int x);

#endif
