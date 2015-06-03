#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#include "defines.h"

/*
 * Display
 */
#include "tm_stm32f4_pcd8544.h"

/*
 * Accelerometer
 */
#include "tm_stm32f4_lis302dl_lis3dsh.h"

/*
 * Global structures
 */
GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;


/*
 * Led enum shorthand
 */
typedef enum {
	LED_ORANGE = GPIO_PIN_13,
	LED_GREEN = GPIO_PIN_12,
	LED_RED = GPIO_PIN_14,
	LED_BLUE = GPIO_PIN_15,
	LED_ALL = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15
} led_t;

/*
 * Prototype declarations
 */
void msGPIO_Init(void);
void msTIM_Init(void);
void LedOff(led_t x);
void LedOn(led_t x);
void delay(int x);

/*
 * Timer handler
 */
int xyz = 0;
void TIM4_IRQHandler(void){
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		if(xyz%2==0){
			LedOn(LED_GREEN);
		}
		else{
			LedOff(LED_GREEN);
		}
		xyz++;
	}
}

int main(void)
{
	/*
	 * Accelerometer data variable
	 */
	TM_LIS302DL_LIS3DSH_t Axes_Data;

	SystemInit();
	SystemCoreClockUpdate();
	msGPIO_Init();
	msTIM_Init();

	/*
	 * Display initialization
	 */
	PCD8544_Init(0x38);
	// 1. accelerometer
	// 2. leds accel output
	// 3. button GPIO
	// 4. game project

	if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
	        LedOn(LED_GREEN | LED_RED);
	        /* Initialize LIS302DL */
	        TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
	        Delay(1000);
	    } else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
	        LedOn(LED_BLUE | LED_ORANGE);
	        /* Initialize LIS3DSH */
	        TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	        Delay(1000);
	    } else {
	        /*
	         * Accelerometer not recognized
	         */
	    	LedOn(LED_ALL);
	    	while(1);
	    }
    while(1)
    {
    	/* Read axes data from initialized accelerometer */
    	TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);

    	/* Turn LEDS on or off */
    	/* Check X axes */
    	if (Axes_Data.X > 400) {
    		LedOn(LED_RED);
    	} else {
    		LedOff(LED_RED);
    	}
    	if (Axes_Data.X < -400) {
    		LedOn(LED_GREEN);
    	} else {
    		//LedOff(LED_GREEN);
    	}
    	/* Check Y axes */
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

	TIM_TimeBaseStructure.TIM_Period= 1250;
	TIM_TimeBaseStructure.TIM_Prescaler= 21000;
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
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
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
	int i;
	for(i=0; i<1000*x; i++);
}
