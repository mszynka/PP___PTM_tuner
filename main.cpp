#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

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
class Point_2d;

// Prototype declarations
void msGPIO_Init(void);
void msTIM_Init(void);
void LedOff(led_t x);
void LedOn(led_t x);
void Delay(int seconds);
void accel_detect(void);
Point_2d accel_state(void);

int ceil(float num) {
    int inum = (int)num;
    if (num == (float)inum) {
        return inum;
    }
    return inum + 1;
}

// Sprite position structure
class Point_2d{
  public:
	int x;
	int y;

	Point_2d(){
		this->x = 0;
		this->y = 0;
	}

    Point_2d(int x, int y){
		this->x = x;
		this->y = y;
	}
};

class Sprite{
  public:
	Point_2d position;

	Sprite(Point_2d position){
		this->position = position;
	}

	Sprite(int x, int y){
		this->position.x = x;
		this->position.y = y;
	}
};

class Display{
	int** maze;
	int** screen;
	Point_2d* startPoint;
	Point_2d* endPoint;
	Sprite* ball;

  public:
	Display(){
		this->maze = new int*[84];
		this->screen = new int*[84];
		for(int x=0; x<84; x++){
			this->maze[x] = new int[48];
			this->screen[x] = new int[48];
			for(int y=0; y<48; y++){
				this->screen[x][y] = 0;
				if((y == 0) or (x == 0) or (x == 83) or (y == 47) or ( x == 17 and y == 1) or ( x == 18 and y == 1) or ( x == 19 and y == 1) or ( x == 20 and y == 1) or ( x == 21 and y == 1) or ( x == 22 and y == 1) or ( x == 23 and y == 1) or ( x == 24 and y == 1) or ( x == 25 and y == 1) or ( x == 26 and y == 1) or ( x == 27 and y == 1) or ( x == 28 and y == 1) or ( x == 29 and y == 1) or ( x == 30 and y == 1) or ( x == 31 and y == 1) or ( x == 32 and y == 1) or ( x == 33 and y == 1) or ( x == 34 and y == 1) or ( x == 35 and y == 1) or ( x == 36 and y == 1) or ( x == 37 and y == 1) or ( x == 38 and y == 1) or ( x == 39 and y == 1) or ( x == 40 and y == 1) or ( x == 41 and y == 1) or ( x == 42 and y == 1) or ( x == 43 and y == 1) or ( x == 44 and y == 1) or ( x == 45 and y == 1) or ( x == 46 and y == 1) or ( x == 56 and y == 1) or ( x == 57 and y == 1) or ( x == 58 and y == 1) or ( x == 59 and y == 1) or ( x == 60 and y == 1) or ( x == 61 and y == 1) or ( x == 62 and y == 1) or ( x == 63 and y == 1) or ( x == 64 and y == 1) or ( x == 65 and y == 1) or ( x == 66 and y == 1) or ( x == 67 and y == 1) or ( x == 68 and y == 1) or ( x == 69 and y == 1) or ( x == 70 and y == 1) or ( x == 71 and y == 1) or ( x == 72 and y == 1) or ( x == 73 and y == 1) or ( x == 74 and y == 1) or ( x == 75 and y == 1) or ( x == 76 and y == 1) or ( x == 77 and y == 1) or ( x == 78 and y == 1) or ( x == 79 and y == 1) or ( x == 80 and y == 1) or ( x == 81 and y == 1) or ( x == 82 and y == 1) or ( x == 19 and y == 2) or ( x == 20 and y == 2) or ( x == 21 and y == 2) or ( x == 22 and y == 2) or ( x == 23 and y == 2) or ( x == 24 and y == 2) or ( x == 25 and y == 2) or ( x == 26 and y == 2) or ( x == 27 and y == 2) or ( x == 28 and y == 2) or ( x == 29 and y == 2) or ( x == 30 and y == 2) or ( x == 31 and y == 2) or ( x == 32 and y == 2) or ( x == 33 and y == 2) or ( x == 34 and y == 2) or ( x == 35 and y == 2) or ( x == 36 and y == 2) or ( x == 37 and y == 2) or ( x == 38 and y == 2) or ( x == 39 and y == 2) or ( x == 40 and y == 2) or ( x == 41 and y == 2) or ( x == 42 and y == 2) or ( x == 43 and y == 2) or ( x == 44 and y == 2) or ( x == 45 and y == 2) or ( x == 61 and y == 2) or ( x == 62 and y == 2) or ( x == 63 and y == 2) or ( x == 64 and y == 2) or ( x == 65 and y == 2) or ( x == 66 and y == 2) or ( x == 67 and y == 2) or ( x == 68 and y == 2) or ( x == 69 and y == 2) or ( x == 70 and y == 2) or ( x == 71 and y == 2) or ( x == 72 and y == 2) or ( x == 73 and y == 2) or ( x == 74 and y == 2) or ( x == 75 and y == 2) or ( x == 76 and y == 2) or ( x == 77 and y == 2) or ( x == 78 and y == 2) or ( x == 79 and y == 2) or ( x == 80 and y == 2) or ( x == 81 and y == 2) or ( x == 82 and y == 2) or ( x == 21 and y == 3) or ( x == 22 and y == 3) or ( x == 23 and y == 3) or ( x == 24 and y == 3) or ( x == 25 and y == 3) or ( x == 26 and y == 3) or ( x == 27 and y == 3) or ( x == 28 and y == 3) or ( x == 29 and y == 3) or ( x == 30 and y == 3) or ( x == 31 and y == 3) or ( x == 32 and y == 3) or ( x == 33 and y == 3) or ( x == 34 and y == 3) or ( x == 35 and y == 3) or ( x == 36 and y == 3) or ( x == 37 and y == 3) or ( x == 38 and y == 3) or ( x == 39 and y == 3) or ( x == 40 and y == 3) or ( x == 41 and y == 3) or ( x == 42 and y == 3) or ( x == 43 and y == 3) or ( x == 44 and y == 3) or ( x == 62 and y == 3) or ( x == 63 and y == 3) or ( x == 64 and y == 3) or ( x == 65 and y == 3) or ( x == 66 and y == 3) or ( x == 67 and y == 3) or ( x == 68 and y == 3) or ( x == 69 and y == 3) or ( x == 70 and y == 3) or ( x == 71 and y == 3) or ( x == 72 and y == 3) or ( x == 73 and y == 3) or ( x == 74 and y == 3) or ( x == 75 and y == 3) or ( x == 76 and y == 3) or ( x == 77 and y == 3) or ( x == 78 and y == 3) or ( x == 79 and y == 3) or ( x == 80 and y == 3) or ( x == 81 and y == 3) or ( x == 82 and y == 3) or ( x == 11 and y == 4) or ( x == 12 and y == 4) or ( x == 13 and y == 4) or ( x == 23 and y == 4) or ( x == 24 and y == 4) or ( x == 25 and y == 4) or ( x == 26 and y == 4) or ( x == 27 and y == 4) or ( x == 28 and y == 4) or ( x == 29 and y == 4) or ( x == 30 and y == 4) or ( x == 31 and y == 4) or ( x == 32 and y == 4) or ( x == 33 and y == 4) or ( x == 34 and y == 4) or ( x == 35 and y == 4) or ( x == 36 and y == 4) or ( x == 37 and y == 4) or ( x == 38 and y == 4) or ( x == 39 and y == 4) or ( x == 40 and y == 4) or ( x == 41 and y == 4) or ( x == 42 and y == 4) or ( x == 43 and y == 4) or ( x == 9 and y == 5) or ( x == 10 and y == 5) or ( x == 11 and y == 5) or ( x == 18 and y == 5) or ( x == 25 and y == 5) or ( x == 26 and y == 5) or ( x == 27 and y == 5) or ( x == 28 and y == 5) or ( x == 29 and y == 5) or ( x == 30 and y == 5) or ( x == 31 and y == 5) or ( x == 32 and y == 5) or ( x == 33 and y == 5) or ( x == 34 and y == 5) or ( x == 35 and y == 5) or ( x == 36 and y == 5) or ( x == 37 and y == 5) or ( x == 38 and y == 5) or ( x == 39 and y == 5) or ( x == 40 and y == 5) or ( x == 41 and y == 5) or ( x == 42 and y == 5) or ( x == 7 and y == 6) or ( x == 8 and y == 6) or ( x == 9 and y == 6) or ( x == 17 and y == 6) or ( x == 18 and y == 6) or ( x == 19 and y == 6) or ( x == 27 and y == 6) or ( x == 28 and y == 6) or ( x == 29 and y == 6) or ( x == 30 and y == 6) or ( x == 31 and y == 6) or ( x == 32 and y == 6) or ( x == 33 and y == 6) or ( x == 34 and y == 6) or ( x == 35 and y == 6) or ( x == 36 and y == 6) or ( x == 37 and y == 6) or ( x == 38 and y == 6) or ( x == 39 and y == 6) or ( x == 40 and y == 6) or ( x == 41 and y == 6) or ( x == 5 and y == 7) or ( x == 6 and y == 7) or ( x == 7 and y == 7) or ( x == 16 and y == 7) or ( x == 17 and y == 7) or ( x == 18 and y == 7) or ( x == 19 and y == 7) or ( x == 20 and y == 7) or ( x == 34 and y == 7) or ( x == 35 and y == 7) or ( x == 63 and y == 7) or ( x == 64 and y == 7) or ( x == 65 and y == 7) or ( x == 66 and y == 7) or ( x == 67 and y == 7) or ( x == 3 and y == 8) or ( x == 4 and y == 8) or ( x == 5 and y == 8) or ( x == 15 and y == 8) or ( x == 16 and y == 8) or ( x == 17 and y == 8) or ( x == 18 and y == 8) or ( x == 19 and y == 8) or ( x == 20 and y == 8) or ( x == 21 and y == 8) or ( x == 34 and y == 8) or ( x == 35 and y == 8) or ( x == 60 and y == 8) or ( x == 61 and y == 8) or ( x == 62 and y == 8) or ( x == 63 and y == 8) or ( x == 64 and y == 8) or ( x == 65 and y == 8) or ( x == 66 and y == 8) or ( x == 67 and y == 8) or ( x == 68 and y == 8) or ( x == 69 and y == 8) or ( x == 70 and y == 8) or ( x == 1 and y == 9) or ( x == 2 and y == 9) or ( x == 3 and y == 9) or ( x == 14 and y == 9) or ( x == 15 and y == 9) or ( x == 16 and y == 9) or ( x == 17 and y == 9) or ( x == 18 and y == 9) or ( x == 19 and y == 9) or ( x == 20 and y == 9) or ( x == 21 and y == 9) or ( x == 22 and y == 9) or ( x == 35 and y == 9) or ( x == 36 and y == 9) or ( x == 54 and y == 9) or ( x == 55 and y == 9) or ( x == 56 and y == 9) or ( x == 57 and y == 9) or ( x == 58 and y == 9) or ( x == 59 and y == 9) or ( x == 60 and y == 9) or ( x == 61 and y == 9) or ( x == 62 and y == 9) or ( x == 63 and y == 9) or ( x == 64 and y == 9) or ( x == 65 and y == 9) or ( x == 66 and y == 9) or ( x == 67 and y == 9) or ( x == 68 and y == 9) or ( x == 69 and y == 9) or ( x == 70 and y == 9) or ( x == 71 and y == 9) or ( x == 72 and y == 9) or ( x == 73 and y == 9) or ( x == 15 and y == 10) or ( x == 16 and y == 10) or ( x == 17 and y == 10) or ( x == 18 and y == 10) or ( x == 19 and y == 10) or ( x == 20 and y == 10) or ( x == 21 and y == 10) or ( x == 22 and y == 10) or ( x == 23 and y == 10) or ( x == 36 and y == 10) or ( x == 37 and y == 10) or ( x == 38 and y == 10) or ( x == 39 and y == 10) or ( x == 40 and y == 10) or ( x == 41 and y == 10) or ( x == 42 and y == 10) or ( x == 43 and y == 10) or ( x == 44 and y == 10) or ( x == 45 and y == 10) or ( x == 46 and y == 10) or ( x == 47 and y == 10) or ( x == 48 and y == 10) or ( x == 49 and y == 10) or ( x == 50 and y == 10) or ( x == 51 and y == 10) or ( x == 52 and y == 10) or ( x == 53 and y == 10) or ( x == 54 and y == 10) or ( x == 55 and y == 10) or ( x == 65 and y == 10) or ( x == 66 and y == 10) or ( x == 67 and y == 10) or ( x == 68 and y == 10) or ( x == 69 and y == 10) or ( x == 70 and y == 10) or ( x == 71 and y == 10) or ( x == 72 and y == 10) or ( x == 78 and y == 10) or ( x == 79 and y == 10) or ( x == 80 and y == 10) or ( x == 81 and y == 10) or ( x == 82 and y == 10) or ( x == 16 and y == 11) or ( x == 17 and y == 11) or ( x == 18 and y == 11) or ( x == 19 and y == 11) or ( x == 20 and y == 11) or ( x == 21 and y == 11) or ( x == 22 and y == 11) or ( x == 23 and y == 11) or ( x == 24 and y == 11) or ( x == 39 and y == 11) or ( x == 40 and y == 11) or ( x == 41 and y == 11) or ( x == 42 and y == 11) or ( x == 43 and y == 11) or ( x == 44 and y == 11) or ( x == 45 and y == 11) or ( x == 46 and y == 11) or ( x == 47 and y == 11) or ( x == 48 and y == 11) or ( x == 49 and y == 11) or ( x == 50 and y == 11) or ( x == 51 and y == 11) or ( x == 52 and y == 11) or ( x == 53 and y == 11) or ( x == 68 and y == 11) or ( x == 69 and y == 11) or ( x == 70 and y == 11) or ( x == 71 and y == 11) or ( x == 80 and y == 11) or ( x == 81 and y == 11) or ( x == 82 and y == 11) or ( x == 17 and y == 12) or ( x == 18 and y == 12) or ( x == 19 and y == 12) or ( x == 20 and y == 12) or ( x == 21 and y == 12) or ( x == 22 and y == 12) or ( x == 23 and y == 12) or ( x == 24 and y == 12) or ( x == 25 and y == 12) or ( x == 26 and y == 12) or ( x == 27 and y == 12) or ( x == 28 and y == 12) or ( x == 29 and y == 12) or ( x == 30 and y == 12) or ( x == 31 and y == 12) or ( x == 32 and y == 12) or ( x == 81 and y == 12) or ( x == 82 and y == 12) or ( x == 1 and y == 13) or ( x == 2 and y == 13) or ( x == 3 and y == 13) or ( x == 4 and y == 13) or ( x == 5 and y == 13) or ( x == 6 and y == 13) or ( x == 18 and y == 13) or ( x == 19 and y == 13) or ( x == 20 and y == 13) or ( x == 21 and y == 13) or ( x == 22 and y == 13) or ( x == 23 and y == 13) or ( x == 24 and y == 13) or ( x == 25 and y == 13) or ( x == 26 and y == 13) or ( x == 27 and y == 13) or ( x == 28 and y == 13) or ( x == 29 and y == 13) or ( x == 30 and y == 13) or ( x == 31 and y == 13) or ( x == 32 and y == 13) or ( x == 33 and y == 13) or ( x == 82 and y == 13) or ( x == 5 and y == 14) or ( x == 6 and y == 14) or ( x == 7 and y == 14) or ( x == 8 and y == 14) or ( x == 9 and y == 14) or ( x == 10 and y == 14) or ( x == 11 and y == 14) or ( x == 12 and y == 14) or ( x == 13 and y == 14) or ( x == 19 and y == 14) or ( x == 20 and y == 14) or ( x == 21 and y == 14) or ( x == 22 and y == 14) or ( x == 23 and y == 14) or ( x == 24 and y == 14) or ( x == 25 and y == 14) or ( x == 26 and y == 14) or ( x == 27 and y == 14) or ( x == 28 and y == 14) or ( x == 29 and y == 14) or ( x == 30 and y == 14) or ( x == 31 and y == 14) or ( x == 32 and y == 14) or ( x == 33 and y == 14) or ( x == 34 and y == 14) or ( x == 35 and y == 14) or ( x == 36 and y == 14) or ( x == 37 and y == 14) or ( x == 38 and y == 14) or ( x == 39 and y == 14) or ( x == 40 and y == 14) or ( x == 41 and y == 14) or ( x == 42 and y == 14) or ( x == 43 and y == 14) or ( x == 44 and y == 14) or ( x == 45 and y == 14) or ( x == 46 and y == 14) or ( x == 47 and y == 14) or ( x == 48 and y == 14) or ( x == 49 and y == 14) or ( x == 50 and y == 14) or ( x == 51 and y == 14) or ( x == 52 and y == 14) or ( x == 53 and y == 14) or ( x == 54 and y == 14) or ( x == 55 and y == 14) or ( x == 56 and y == 14) or ( x == 57 and y == 14) or ( x == 58 and y == 14) or ( x == 59 and y == 14) or ( x == 60 and y == 14) or ( x == 61 and y == 14) or ( x == 62 and y == 14) or ( x == 63 and y == 14) or ( x == 64 and y == 14) or ( x == 65 and y == 14) or ( x == 72 and y == 14) or ( x == 73 and y == 14) or ( x == 74 and y == 14) or ( x == 75 and y == 14) or ( x == 76 and y == 14) or ( x == 77 and y == 14) or ( x == 9 and y == 15) or ( x == 10 and y == 15) or ( x == 11 and y == 15) or ( x == 12 and y == 15) or ( x == 13 and y == 15) or ( x == 20 and y == 15) or ( x == 21 and y == 15) or ( x == 22 and y == 15) or ( x == 23 and y == 15) or ( x == 24 and y == 15) or ( x == 25 and y == 15) or ( x == 26 and y == 15) or ( x == 27 and y == 15) or ( x == 28 and y == 15) or ( x == 29 and y == 15) or ( x == 30 and y == 15) or ( x == 31 and y == 15) or ( x == 32 and y == 15) or ( x == 33 and y == 15) or ( x == 34 and y == 15) or ( x == 35 and y == 15) or ( x == 36 and y == 15) or ( x == 37 and y == 15) or ( x == 38 and y == 15) or ( x == 39 and y == 15) or ( x == 40 and y == 15) or ( x == 41 and y == 15) or ( x == 42 and y == 15) or ( x == 43 and y == 15) or ( x == 44 and y == 15) or ( x == 45 and y == 15) or ( x == 46 and y == 15) or ( x == 47 and y == 15) or ( x == 48 and y == 15) or ( x == 49 and y == 15) or ( x == 50 and y == 15) or ( x == 51 and y == 15) or ( x == 52 and y == 15) or ( x == 53 and y == 15) or ( x == 54 and y == 15) or ( x == 55 and y == 15) or ( x == 56 and y == 15) or ( x == 57 and y == 15) or ( x == 58 and y == 15) or ( x == 59 and y == 15) or ( x == 60 and y == 15) or ( x == 61 and y == 15) or ( x == 62 and y == 15) or ( x == 63 and y == 15) or ( x == 75 and y == 15) or ( x == 76 and y == 15) or ( x == 77 and y == 15) or ( x == 78 and y == 15) or ( x == 79 and y == 15) or ( x == 80 and y == 15) or ( x == 81 and y == 15) or ( x == 82 and y == 15) or ( x == 53 and y == 16) or ( x == 54 and y == 16) or ( x == 55 and y == 16) or ( x == 56 and y == 16) or ( x == 57 and y == 16) or ( x == 58 and y == 16) or ( x == 59 and y == 16) or ( x == 60 and y == 16) or ( x == 61 and y == 16) or ( x == 62 and y == 16) or ( x == 78 and y == 16) or ( x == 79 and y == 16) or ( x == 80 and y == 16) or ( x == 81 and y == 16) or ( x == 82 and y == 16) or ( x == 11 and y == 17) or ( x == 12 and y == 17) or ( x == 13 and y == 17) or ( x == 14 and y == 17) or ( x == 15 and y == 17) or ( x == 16 and y == 17) or ( x == 54 and y == 17) or ( x == 55 and y == 17) or ( x == 56 and y == 17) or ( x == 57 and y == 17) or ( x == 58 and y == 17) or ( x == 59 and y == 17) or ( x == 60 and y == 17) or ( x == 80 and y == 17) or ( x == 81 and y == 17) or ( x == 82 and y == 17) or ( x == 13 and y == 18) or ( x == 14 and y == 18) or ( x == 15 and y == 18) or ( x == 16 and y == 18) or ( x == 17 and y == 18) or ( x == 18 and y == 18) or ( x == 19 and y == 18) or ( x == 20 and y == 18) or ( x == 21 and y == 18) or ( x == 22 and y == 18) or ( x == 23 and y == 18) or ( x == 24 and y == 18) or ( x == 55 and y == 18) or ( x == 56 and y == 18) or ( x == 57 and y == 18) or ( x == 58 and y == 18) or ( x == 74 and y == 18) or ( x == 75 and y == 18) or ( x == 76 and y == 18) or ( x == 77 and y == 18) or ( x == 78 and y == 18) or ( x == 79 and y == 18) or ( x == 80 and y == 18) or ( x == 81 and y == 18) or ( x == 82 and y == 18) or ( x == 3 and y == 19) or ( x == 4 and y == 19) or ( x == 5 and y == 19) or ( x == 15 and y == 19) or ( x == 16 and y == 19) or ( x == 17 and y == 19) or ( x == 18 and y == 19) or ( x == 19 and y == 19) or ( x == 20 and y == 19) or ( x == 21 and y == 19) or ( x == 22 and y == 19) or ( x == 30 and y == 19) or ( x == 31 and y == 19) or ( x == 32 and y == 19) or ( x == 39 and y == 19) or ( x == 40 and y == 19) or ( x == 41 and y == 19) or ( x == 42 and y == 19) or ( x == 43 and y == 19) or ( x == 44 and y == 19) or ( x == 56 and y == 19) or ( x == 57 and y == 19) or ( x == 75 and y == 19) or ( x == 76 and y == 19) or ( x == 77 and y == 19) or ( x == 78 and y == 19) or ( x == 79 and y == 19) or ( x == 80 and y == 19) or ( x == 81 and y == 19) or ( x == 82 and y == 19) or ( x == 5 and y == 20) or ( x == 6 and y == 20) or ( x == 7 and y == 20) or ( x == 17 and y == 20) or ( x == 18 and y == 20) or ( x == 19 and y == 20) or ( x == 20 and y == 20) or ( x == 21 and y == 20) or ( x == 28 and y == 20) or ( x == 29 and y == 20) or ( x == 30 and y == 20) or ( x == 41 and y == 20) or ( x == 42 and y == 20) or ( x == 43 and y == 20) or ( x == 44 and y == 20) or ( x == 56 and y == 20) or ( x == 57 and y == 20) or ( x == 66 and y == 20) or ( x == 67 and y == 20) or ( x == 76 and y == 20) or ( x == 77 and y == 20) or ( x == 78 and y == 20) or ( x == 79 and y == 20) or ( x == 80 and y == 20) or ( x == 81 and y == 20) or ( x == 82 and y == 20) or ( x == 7 and y == 21) or ( x == 8 and y == 21) or ( x == 9 and y == 21) or ( x == 18 and y == 21) or ( x == 19 and y == 21) or ( x == 20 and y == 21) or ( x == 26 and y == 21) or ( x == 27 and y == 21) or ( x == 28 and y == 21) or ( x == 41 and y == 21) or ( x == 42 and y == 21) or ( x == 43 and y == 21) or ( x == 44 and y == 21) or ( x == 45 and y == 21) or ( x == 46 and y == 21) or ( x == 56 and y == 21) or ( x == 57 and y == 21) or ( x == 65 and y == 21) or ( x == 66 and y == 21) or ( x == 67 and y == 21) or ( x == 68 and y == 21) or ( x == 77 and y == 21) or ( x == 78 and y == 21) or ( x == 79 and y == 21) or ( x == 80 and y == 21) or ( x == 81 and y == 21) or ( x == 82 and y == 21) or ( x == 9 and y == 22) or ( x == 10 and y == 22) or ( x == 11 and y == 22) or ( x == 24 and y == 22) or ( x == 25 and y == 22) or ( x == 26 and y == 22) or ( x == 41 and y == 22) or ( x == 42 and y == 22) or ( x == 43 and y == 22) or ( x == 44 and y == 22) or ( x == 45 and y == 22) or ( x == 46 and y == 22) or ( x == 47 and y == 22) or ( x == 56 and y == 22) or ( x == 57 and y == 22) or ( x == 64 and y == 22) or ( x == 65 and y == 22) or ( x == 66 and y == 22) or ( x == 67 and y == 22) or ( x == 68 and y == 22) or ( x == 69 and y == 22) or ( x == 78 and y == 22) or ( x == 79 and y == 22) or ( x == 80 and y == 22) or ( x == 81 and y == 22) or ( x == 82 and y == 22) or ( x == 11 and y == 23) or ( x == 12 and y == 23) or ( x == 13 and y == 23) or ( x == 22 and y == 23) or ( x == 23 and y == 23) or ( x == 24 and y == 23) or ( x == 56 and y == 23) or ( x == 57 and y == 23) or ( x == 63 and y == 23) or ( x == 64 and y == 23) or ( x == 65 and y == 23) or ( x == 66 and y == 23) or ( x == 67 and y == 23) or ( x == 68 and y == 23) or ( x == 69 and y == 23) or ( x == 70 and y == 23) or ( x == 79 and y == 23) or ( x == 80 and y == 23) or ( x == 81 and y == 23) or ( x == 82 and y == 23) or ( x == 12 and y == 24) or ( x == 13 and y == 24) or ( x == 22 and y == 24) or ( x == 23 and y == 24) or ( x == 32 and y == 24) or ( x == 33 and y == 24) or ( x == 34 and y == 24) or ( x == 35 and y == 24) or ( x == 56 and y == 24) or ( x == 57 and y == 24) or ( x == 62 and y == 24) or ( x == 63 and y == 24) or ( x == 64 and y == 24) or ( x == 65 and y == 24) or ( x == 66 and y == 24) or ( x == 67 and y == 24) or ( x == 68 and y == 24) or ( x == 69 and y == 24) or ( x == 70 and y == 24) or ( x == 71 and y == 24) or ( x == 80 and y == 24) or ( x == 81 and y == 24) or ( x == 82 and y == 24) or ( x == 12 and y == 25) or ( x == 13 and y == 25) or ( x == 22 and y == 25) or ( x == 23 and y == 25) or ( x == 31 and y == 25) or ( x == 32 and y == 25) or ( x == 33 and y == 25) or ( x == 34 and y == 25) or ( x == 47 and y == 25) or ( x == 48 and y == 25) or ( x == 49 and y == 25) or ( x == 50 and y == 25) or ( x == 61 and y == 25) or ( x == 62 and y == 25) or ( x == 63 and y == 25) or ( x == 64 and y == 25) or ( x == 65 and y == 25) or ( x == 66 and y == 25) or ( x == 67 and y == 25) or ( x == 68 and y == 25) or ( x == 69 and y == 25) or ( x == 70 and y == 25) or ( x == 71 and y == 25) or ( x == 72 and y == 25) or ( x == 81 and y == 25) or ( x == 82 and y == 25) or ( x == 12 and y == 26) or ( x == 13 and y == 26) or ( x == 22 and y == 26) or ( x == 23 and y == 26) or ( x == 30 and y == 26) or ( x == 31 and y == 26) or ( x == 32 and y == 26) or ( x == 33 and y == 26) or ( x == 47 and y == 26) or ( x == 48 and y == 26) or ( x == 49 and y == 26) or ( x == 50 and y == 26) or ( x == 60 and y == 26) or ( x == 61 and y == 26) or ( x == 62 and y == 26) or ( x == 63 and y == 26) or ( x == 64 and y == 26) or ( x == 65 and y == 26) or ( x == 66 and y == 26) or ( x == 67 and y == 26) or ( x == 68 and y == 26) or ( x == 69 and y == 26) or ( x == 70 and y == 26) or ( x == 71 and y == 26) or ( x == 72 and y == 26) or ( x == 73 and y == 26) or ( x == 82 and y == 26) or ( x == 12 and y == 27) or ( x == 13 and y == 27) or ( x == 22 and y == 27) or ( x == 23 and y == 27) or ( x == 31 and y == 27) or ( x == 32 and y == 27) or ( x == 33 and y == 27) or ( x == 34 and y == 27) or ( x == 59 and y == 27) or ( x == 60 and y == 27) or ( x == 61 and y == 27) or ( x == 62 and y == 27) or ( x == 63 and y == 27) or ( x == 64 and y == 27) or ( x == 65 and y == 27) or ( x == 66 and y == 27) or ( x == 67 and y == 27) or ( x == 68 and y == 27) or ( x == 69 and y == 27) or ( x == 70 and y == 27) or ( x == 71 and y == 27) or ( x == 72 and y == 27) or ( x == 73 and y == 27) or ( x == 74 and y == 27) or ( x == 12 and y == 28) or ( x == 13 and y == 28) or ( x == 22 and y == 28) or ( x == 23 and y == 28) or ( x == 33 and y == 28) or ( x == 34 and y == 28) or ( x == 35 and y == 28) or ( x == 36 and y == 28) or ( x == 42 and y == 28) or ( x == 43 and y == 28) or ( x == 59 and y == 28) or ( x == 60 and y == 28) or ( x == 61 and y == 28) or ( x == 62 and y == 28) or ( x == 63 and y == 28) or ( x == 64 and y == 28) or ( x == 65 and y == 28) or ( x == 66 and y == 28) or ( x == 67 and y == 28) or ( x == 68 and y == 28) or ( x == 69 and y == 28) or ( x == 70 and y == 28) or ( x == 71 and y == 28) or ( x == 72 and y == 28) or ( x == 73 and y == 28) or ( x == 74 and y == 28) or ( x == 75 and y == 28) or ( x == 76 and y == 28) or ( x == 77 and y == 28) or ( x == 78 and y == 28) or ( x == 11 and y == 29) or ( x == 12 and y == 29) or ( x == 13 and y == 29) or ( x == 22 and y == 29) or ( x == 23 and y == 29) or ( x == 24 and y == 29) or ( x == 42 and y == 29) or ( x == 43 and y == 29) or ( x == 47 and y == 29) or ( x == 48 and y == 29) or ( x == 49 and y == 29) or ( x == 50 and y == 29) or ( x == 51 and y == 29) or ( x == 59 and y == 29) or ( x == 60 and y == 29) or ( x == 61 and y == 29) or ( x == 62 and y == 29) or ( x == 63 and y == 29) or ( x == 64 and y == 29) or ( x == 65 and y == 29) or ( x == 66 and y == 29) or ( x == 67 and y == 29) or ( x == 68 and y == 29) or ( x == 69 and y == 29) or ( x == 70 and y == 29) or ( x == 71 and y == 29) or ( x == 72 and y == 29) or ( x == 73 and y == 29) or ( x == 74 and y == 29) or ( x == 75 and y == 29) or ( x == 76 and y == 29) or ( x == 77 and y == 29) or ( x == 78 and y == 29) or ( x == 9 and y == 30) or ( x == 10 and y == 30) or ( x == 11 and y == 30) or ( x == 24 and y == 30) or ( x == 25 and y == 30) or ( x == 26 and y == 30) or ( x == 42 and y == 30) or ( x == 43 and y == 30) or ( x == 49 and y == 30) or ( x == 50 and y == 30) or ( x == 51 and y == 30) or ( x == 77 and y == 30) or ( x == 78 and y == 30) or ( x == 7 and y == 31) or ( x == 8 and y == 31) or ( x == 9 and y == 31) or ( x == 26 and y == 31) or ( x == 27 and y == 31) or ( x == 28 and y == 31) or ( x == 42 and y == 31) or ( x == 43 and y == 31) or ( x == 49 and y == 31) or ( x == 50 and y == 31) or ( x == 51 and y == 31) or ( x == 52 and y == 31) or ( x == 77 and y == 31) or ( x == 78 and y == 31) or ( x == 5 and y == 32) or ( x == 6 and y == 32) or ( x == 7 and y == 32) or ( x == 28 and y == 32) or ( x == 29 and y == 32) or ( x == 30 and y == 32) or ( x == 42 and y == 32) or ( x == 43 and y == 32) or ( x == 44 and y == 32) or ( x == 57 and y == 32) or ( x == 58 and y == 32) or ( x == 59 and y == 32) or ( x == 60 and y == 32) or ( x == 61 and y == 32) or ( x == 77 and y == 32) or ( x == 78 and y == 32) or ( x == 3 and y == 33) or ( x == 4 and y == 33) or ( x == 5 and y == 33) or ( x == 30 and y == 33) or ( x == 31 and y == 33) or ( x == 32 and y == 33) or ( x == 42 and y == 33) or ( x == 43 and y == 33) or ( x == 44 and y == 33) or ( x == 45 and y == 33) or ( x == 52 and y == 33) or ( x == 53 and y == 33) or ( x == 54 and y == 33) or ( x == 55 and y == 33) or ( x == 56 and y == 33) or ( x == 57 and y == 33) or ( x == 58 and y == 33) or ( x == 59 and y == 33) or ( x == 60 and y == 33) or ( x == 61 and y == 33) or ( x == 62 and y == 33) or ( x == 63 and y == 33) or ( x == 77 and y == 33) or ( x == 78 and y == 33) or ( x == 32 and y == 34) or ( x == 33 and y == 34) or ( x == 34 and y == 34) or ( x == 42 and y == 34) or ( x == 43 and y == 34) or ( x == 44 and y == 34) or ( x == 45 and y == 34) or ( x == 46 and y == 34) or ( x == 53 and y == 34) or ( x == 54 and y == 34) or ( x == 55 and y == 34) or ( x == 56 and y == 34) or ( x == 57 and y == 34) or ( x == 58 and y == 34) or ( x == 59 and y == 34) or ( x == 60 and y == 34) or ( x == 61 and y == 34) or ( x == 62 and y == 34) or ( x == 63 and y == 34) or ( x == 64 and y == 34) or ( x == 65 and y == 34) or ( x == 77 and y == 34) or ( x == 78 and y == 34) or ( x == 13 and y == 35) or ( x == 14 and y == 35) or ( x == 15 and y == 35) or ( x == 16 and y == 35) or ( x == 17 and y == 35) or ( x == 18 and y == 35) or ( x == 19 and y == 35) or ( x == 20 and y == 35) or ( x == 21 and y == 35) or ( x == 22 and y == 35) or ( x == 42 and y == 35) or ( x == 43 and y == 35) or ( x == 44 and y == 35) or ( x == 45 and y == 35) or ( x == 46 and y == 35) or ( x == 47 and y == 35) or ( x == 77 and y == 35) or ( x == 78 and y == 35) or ( x == 11 and y == 36) or ( x == 12 and y == 36) or ( x == 13 and y == 36) or ( x == 14 and y == 36) or ( x == 15 and y == 36) or ( x == 16 and y == 36) or ( x == 17 and y == 36) or ( x == 18 and y == 36) or ( x == 19 and y == 36) or ( x == 20 and y == 36) or ( x == 21 and y == 36) or ( x == 22 and y == 36) or ( x == 23 and y == 36) or ( x == 24 and y == 36) or ( x == 42 and y == 36) or ( x == 43 and y == 36) or ( x == 44 and y == 36) or ( x == 45 and y == 36) or ( x == 46 and y == 36) or ( x == 47 and y == 36) or ( x == 48 and y == 36) or ( x == 77 and y == 36) or ( x == 78 and y == 36) or ( x == 11 and y == 37) or ( x == 12 and y == 37) or ( x == 13 and y == 37) or ( x == 14 and y == 37) or ( x == 15 and y == 37) or ( x == 16 and y == 37) or ( x == 17 and y == 37) or ( x == 18 and y == 37) or ( x == 19 and y == 37) or ( x == 20 and y == 37) or ( x == 21 and y == 37) or ( x == 22 and y == 37) or ( x == 23 and y == 37) or ( x == 24 and y == 37) or ( x == 25 and y == 37) or ( x == 26 and y == 37) or ( x == 27 and y == 37) or ( x == 42 and y == 37) or ( x == 43 and y == 37) or ( x == 44 and y == 37) or ( x == 45 and y == 37) or ( x == 46 and y == 37) or ( x == 47 and y == 37) or ( x == 48 and y == 37) or ( x == 49 and y == 37) or ( x == 77 and y == 37) or ( x == 78 and y == 37) or ( x == 13 and y == 38) or ( x == 14 and y == 38) or ( x == 15 and y == 38) or ( x == 16 and y == 38) or ( x == 17 and y == 38) or ( x == 18 and y == 38) or ( x == 19 and y == 38) or ( x == 20 and y == 38) or ( x == 21 and y == 38) or ( x == 22 and y == 38) or ( x == 23 and y == 38) or ( x == 24 and y == 38) or ( x == 25 and y == 38) or ( x == 26 and y == 38) or ( x == 27 and y == 38) or ( x == 28 and y == 38) or ( x == 29 and y == 38) or ( x == 42 and y == 38) or ( x == 43 and y == 38) or ( x == 44 and y == 38) or ( x == 45 and y == 38) or ( x == 46 and y == 38) or ( x == 47 and y == 38) or ( x == 48 and y == 38) or ( x == 49 and y == 38) or ( x == 50 and y == 38) or ( x == 51 and y == 38) or ( x == 52 and y == 38) or ( x == 53 and y == 38) or ( x == 54 and y == 38) or ( x == 55 and y == 38) or ( x == 56 and y == 38) or ( x == 57 and y == 38) or ( x == 58 and y == 38) or ( x == 59 and y == 38) or ( x == 60 and y == 38) or ( x == 61 and y == 38) or ( x == 62 and y == 38) or ( x == 63 and y == 38) or ( x == 64 and y == 38) or ( x == 65 and y == 38) or ( x == 66 and y == 38) or ( x == 67 and y == 38) or ( x == 68 and y == 38) or ( x == 69 and y == 38) or ( x == 77 and y == 38) or ( x == 78 and y == 38) or ( x == 3 and y == 39) or ( x == 4 and y == 39) or ( x == 5 and y == 39) or ( x == 6 and y == 39) or ( x == 7 and y == 39) or ( x == 8 and y == 39) or ( x == 9 and y == 39) or ( x == 10 and y == 39) or ( x == 11 and y == 39) or ( x == 12 and y == 39) or ( x == 13 and y == 39) or ( x == 14 and y == 39) or ( x == 15 and y == 39) or ( x == 16 and y == 39) or ( x == 17 and y == 39) or ( x == 18 and y == 39) or ( x == 19 and y == 39) or ( x == 20 and y == 39) or ( x == 21 and y == 39) or ( x == 22 and y == 39) or ( x == 23 and y == 39) or ( x == 24 and y == 39) or ( x == 25 and y == 39) or ( x == 26 and y == 39) or ( x == 27 and y == 39) or ( x == 28 and y == 39) or ( x == 29 and y == 39) or ( x == 30 and y == 39) or ( x == 31 and y == 39) or ( x == 32 and y == 39) or ( x == 42 and y == 39) or ( x == 43 and y == 39) or ( x == 44 and y == 39) or ( x == 45 and y == 39) or ( x == 46 and y == 39) or ( x == 47 and y == 39) or ( x == 48 and y == 39) or ( x == 49 and y == 39) or ( x == 50 and y == 39) or ( x == 51 and y == 39) or ( x == 52 and y == 39) or ( x == 53 and y == 39) or ( x == 54 and y == 39) or ( x == 55 and y == 39) or ( x == 56 and y == 39) or ( x == 57 and y == 39) or ( x == 58 and y == 39) or ( x == 59 and y == 39) or ( x == 60 and y == 39) or ( x == 61 and y == 39) or ( x == 62 and y == 39) or ( x == 63 and y == 39) or ( x == 64 and y == 39) or ( x == 65 and y == 39) or ( x == 66 and y == 39) or ( x == 67 and y == 39) or ( x == 68 and y == 39) or ( x == 69 and y == 39) or ( x == 1 and y == 40) or ( x == 2 and y == 40) or ( x == 3 and y == 40) or ( x == 4 and y == 40) or ( x == 1 and y == 41) or ( x == 2 and y == 41) or ( x == 3 and y == 41) or ( x == 4 and y == 41) or ( x == 1 and y == 42) or ( x == 2 and y == 42) or ( x == 3 and y == 42) or ( x == 4 and y == 42) or ( x == 5 and y == 42) or ( x == 6 and y == 42) or ( x == 7 and y == 42) or ( x == 8 and y == 42) or ( x == 9 and y == 42) or ( x == 10 and y == 42) or ( x == 11 and y == 42) or ( x == 12 and y == 42) or ( x == 13 and y == 42) or ( x == 14 and y == 42) or ( x == 15 and y == 42) or ( x == 16 and y == 42) or ( x == 17 and y == 42) or ( x == 18 and y == 42) or ( x == 19 and y == 42) or ( x == 20 and y == 42) or ( x == 21 and y == 42) or ( x == 22 and y == 42) or ( x == 23 and y == 42) or ( x == 24 and y == 42) or ( x == 25 and y == 42) or ( x == 26 and y == 42) or ( x == 27 and y == 42) or ( x == 28 and y == 42) or ( x == 29 and y == 42) or ( x == 30 and y == 42) or ( x == 31 and y == 42) or ( x == 32 and y == 42) or ( x == 33 and y == 42) or ( x == 34 and y == 42) or ( x == 35 and y == 42) or ( x == 36 and y == 42) or ( x == 37 and y == 42) or ( x == 38 and y == 42) or ( x == 39 and y == 42) or ( x == 47 and y == 42) or ( x == 48 and y == 42) or ( x == 49 and y == 42) or ( x == 50 and y == 42) or ( x == 51 and y == 42) or ( x == 52 and y == 42) or ( x == 53 and y == 42) or ( x == 54 and y == 42) or ( x == 55 and y == 42) or ( x == 56 and y == 42) or ( x == 57 and y == 42) or ( x == 58 and y == 42) or ( x == 59 and y == 42) or ( x == 60 and y == 42) or ( x == 61 and y == 42) or ( x == 62 and y == 42) or ( x == 63 and y == 42) or ( x == 64 and y == 42) or ( x == 65 and y == 42) or ( x == 66 and y == 42) or ( x == 67 and y == 42) or ( x == 68 and y == 42) or ( x == 69 and y == 42) or ( x == 70 and y == 42) or ( x == 71 and y == 42) or ( x == 72 and y == 42) or ( x == 73 and y == 42) or ( x == 74 and y == 42) or ( x == 75 and y == 42) or ( x == 76 and y == 42) or ( x == 77 and y == 42) or ( x == 1 and y == 43) or ( x == 2 and y == 43) or ( x == 3 and y == 43) or ( x == 4 and y == 43) or ( x == 5 and y == 43) or ( x == 6 and y == 43) or ( x == 7 and y == 43) or ( x == 8 and y == 43) or ( x == 9 and y == 43) or ( x == 10 and y == 43) or ( x == 11 and y == 43) or ( x == 12 and y == 43) or ( x == 13 and y == 43) or ( x == 14 and y == 43) or ( x == 15 and y == 43) or ( x == 16 and y == 43) or ( x == 17 and y == 43) or ( x == 18 and y == 43) or ( x == 19 and y == 43) or ( x == 20 and y == 43) or ( x == 21 and y == 43) or ( x == 22 and y == 43) or ( x == 23 and y == 43) or ( x == 24 and y == 43) or ( x == 25 and y == 43) or ( x == 26 and y == 43) or ( x == 27 and y == 43) or ( x == 28 and y == 43) or ( x == 29 and y == 43) or ( x == 30 and y == 43) or ( x == 31 and y == 43) or ( x == 32 and y == 43) or ( x == 33 and y == 43) or ( x == 34 and y == 43) or ( x == 35 and y == 43) or ( x == 36 and y == 43) or ( x == 37 and y == 43) or ( x == 38 and y == 43) or ( x == 39 and y == 43) or ( x == 47 and y == 43) or ( x == 48 and y == 43) or ( x == 49 and y == 43) or ( x == 50 and y == 43) or ( x == 51 and y == 43) or ( x == 52 and y == 43) or ( x == 53 and y == 43) or ( x == 54 and y == 43) or ( x == 55 and y == 43) or ( x == 56 and y == 43) or ( x == 57 and y == 43) or ( x == 58 and y == 43) or ( x == 59 and y == 43) or ( x == 60 and y == 43) or ( x == 61 and y == 43) or ( x == 62 and y == 43) or ( x == 63 and y == 43) or ( x == 64 and y == 43) or ( x == 65 and y == 43) or ( x == 66 and y == 43) or ( x == 67 and y == 43) or ( x == 68 and y == 43) or ( x == 69 and y == 43) or ( x == 70 and y == 43) or ( x == 71 and y == 43) or ( x == 72 and y == 43) or ( x == 73 and y == 43) or ( x == 74 and y == 43) or ( x == 75 and y == 43) or ( x == 76 and y == 43) or ( x == 77 and y == 43) or ( x == 1 and y == 44) or ( x == 2 and y == 44) or ( x == 3 and y == 44) or ( x == 4 and y == 44) or ( x == 5 and y == 44) or ( x == 6 and y == 44) or ( x == 7 and y == 44) or ( x == 1 and y == 45) or ( x == 2 and y == 45) or ( x == 3 and y == 45) or ( x == 4 and y == 45) or ( x == 5 and y == 45) or ( x == 6 and y == 45) or ( x == 7 and y == 45) or ( x == 8 and y == 45) or ( x == 9 and y == 45) or ( x == 1 and y == 46) or ( x == 2 and y == 46) or ( x == 3 and y == 46) or ( x == 4 and y == 46) or ( x == 5 and y == 46) or ( x == 6 and y == 46) or ( x == 7 and y == 46) or ( x == 8 and y == 46) or ( x == 9 and y == 46) or ( x == 10 and y == 46)){
					this->maze[x][y] = 1;
				}
				else {
					this->maze[x][y] = 0;
				}
			}
		}
		this->startPoint = new Point_2d(80, 44);
		this->endPoint = new Point_2d(3,3);
		this->ball = new Sprite(*this->startPoint);
	}

	void setSpritePosition(Point_2d position){
		if(this-maze[position.x][position.y] == 0 and this->maze[position.x+1][position.y] == 0 and this->maze[position.x][position.y+1] == 0 and this->maze[position.x-1][position.y] == 0 and this->maze[position.x][position.y-1] == 0){
			this->ball->position = position;
			redrawScene();
		}
	}

	bool moveSprite(Point_2d direction){
		if(this->maze[this->ball->position.x-direction.x][this->ball->position.y-direction.y] == 0){
			this->ball->position.x-=direction.x;
			this->ball->position.y-=direction.y;
			this->redrawScene();
			if(this->ball->position.x == this->endPoint->x and this->ball->position.y == this->endPoint->y){
				this->blackScreen();
				Delay(100);
				this->whiteScreen();
			}
			return true;
		}
		else return false;
	}

	void redrawScene(){
		this->screen = this->maze;
		this->screen[this->ball->position.x][this->ball->position.y] = 1;
		this->screen[this->ball->position.x+1][this->ball->position.y] = 1;
		this->screen[this->ball->position.x][this->ball->position.y+1] = 1;
		this->screen[this->ball->position.x-1][this->ball->position.y] = 1;
		this->screen[this->ball->position.x][this->ball->position.y-1] = 1;
		for(int x = 0; x<84; x++){
			for(int y = 0; y<48; y++){
				if(this->screen[x][y] == 1)
					PCD8544_DrawPixel(x,y,PCD8544_Pixel_Set);
				else
					PCD8544_DrawPixel(x,y,PCD8544_Pixel_Clear);
			}
		}
	}

	void blackScreen(){
		for(int x = 0; x<84; x++){
			for(int y = 0; y<48; y++){
				this->screen[x][y] = 1;
			}
		}
		this->redrawScene();
	}

	void whiteScreen(){
		for(int x = 0; x<84; x++){
			for(int y = 0; y<48; y++){
				this->screen[x][y] = 0;
			}
		}
		this->redrawScene();
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

static Display screen;

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	msGPIO_Init();
	msTIM_Init();
	PCD8544_Init(0x38);
	accel_detect();

	// 1. accelerometer			DONE
	// 2. leds accel output		DONE
	// 3. button GPIO
	// 4. game project

	// Application life span
    while(1)
    {
    	Point_2d direction = accel_state();
    	screen.moveSprite(direction);
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
void Delay(int seconds){
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
	delay_timer_on = true;
	delay_timer_time = seconds;
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
		Delay(100);
	} else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
		LedOn(LED_BLUE);
		LedOn(LED_ORANGE);
		/* Initialize LIS3DSH */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
		Delay(100);
	} else {
		// Accelerometer not recognized
		LedOn(LED_ALL);
		while(1);
	}
}

/*
 * Reads the state of accelerometer and translates it to position difference
 */
Point_2d accel_state(){
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
	return Point_2d(x,y);
}
