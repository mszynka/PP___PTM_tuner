#include <stdio.h>
#include <stdlib.h>
//#include <list.h>
#include <math.h>

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#define TIMER_TIMES_PER_SECOND 10

// Display & accelerometer
extern "C"
{
	#include "defines.h"
	#include "tm_stm32f4_pcd8544.h"
	#include "tm_stm32f4_lis302dl_lis3dsh.h"
}

// Global structures
static GPIO_InitTypeDef GPIO_InitStruct;
static TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
static TM_LIS302DL_LIS3DSH_t Axes_Data;
static bool delay_timer_on = true;
static int delay_timer_time;

// Led enum shorthand
typedef enum {
	LED_ORANGE = GPIO_PIN_13,
	LED_GREEN = GPIO_PIN_12,
	LED_RED = GPIO_PIN_14,
	LED_BLUE = GPIO_PIN_15,
	LED_ALL = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15
} led_t;

// Class declarations
class TimerTask;
class Position;

// Prototype declarations
void msGPIO_Init(void);
void msTIM_Init(void);
void LedOff(led_t x);
void LedOn(led_t x);
void Delay(int x);
void accel_detect(void);
Position accel_state(void);

// Sprite position structure
class Position{
	int x;
	int y;

  public:
	Position(int x, int y){
		this->x = x;
		this->y = y;
	}
};

void TIM4_IRQHandler(void){
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

		if(delay_timer_time > 0){
			delay_timer_time--;
		}
		else{
			delay_timer_on = false;
		}
	}
}

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	msGPIO_Init();
	msTIM_Init();
	PCD8544_Init(0x38);
	accel_detect();

	// 1. accelerometer
	// 2. leds accel output
	// 3. button GPIO
	// 4. game project

	// Application life span
    while(1)
    {
    	accel_state();
    }
    return 0;
}

/*
 * GPIO led initialization
 */
void msGPIO_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	LedOff(LED_ALL);
}

/*
 * Timer initialization
 */
void msTIM_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	TIM_TimeBaseStructure.TIM_Period= 4000;
	TIM_TimeBaseStructure.TIM_Prescaler= 2100;
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM4,ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}

/*
 * Led off shorthand
 */
void LedOff(led_t x){
	GPIO_ResetBits(GPIOD, x);
}

/*
 * Led on shorthand
 */
void LedOn(led_t x){
	GPIO_SetBits(GPIOD, x);
}

/*
 * Delay of some sort
 */
void Delay(int x){
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
	delay_timer_on = true;
	delay_timer_time = x/10;
	while(delay_timer_on);
	TIM_ITConfig(TIM4, TIM_IT_Update,DISABLE);
}

/*
 * Detects type of accelerometer accessible and initializes driver
 */
void accel_detect(void){
	if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
		LedOn(LED_GREEN);
		LedOn(LED_RED);
		/* Initialize LIS302DL */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
		Delay(1000);
	} else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
		LedOn(LED_BLUE);
		LedOn(LED_ORANGE);
		/* Initialize LIS3DSH */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
		Delay(1000);
	} else {
		// Accelerometer not recognized
		LedOn(LED_ALL);
		while(1);
	}
}

/*
 * Reads the state of accelerometer and translates it to position difference
 */
Position accel_state(){
	/* Read axes data from initialized accelerometer */
	TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);
	int x = ceil(Axes_Data.X/100);
	int y = ceil(Axes_Data.Y/100);

	if (Axes_Data.X > 400) {
		LedOn(LED_RED);
	} else {
		LedOff(LED_RED);
	}
	if (Axes_Data.X < -400) {
		LedOn(LED_GREEN);
	} else {
		LedOff(LED_GREEN);
	}
	if (Axes_Data.Y > 400) {
		LedOn(LED_ORANGE);
	} else {
		LedOff(LED_ORANGE);
	}
	if (Axes_Data.Y < -400) {
		LedOn(LED_BLUE);
	} else {
		LedOff(LED_BLUE);
	}
	return Position(x,y);
}
