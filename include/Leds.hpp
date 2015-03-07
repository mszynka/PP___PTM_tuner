#ifndef LEDS_HPP
#define LEDS_HPP

#include <bitset>

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

/**
 * Contains convenient names for LEDs.
 */
enum class Led : uint16_t {
	Green = GPIO_PIN_12,
	Orange = GPIO_PIN_13,
	Red = GPIO_PIN_14,
	Blue = GPIO_PIN_15
};

/**
 * Implements basic operations with on-board leds.
 * Requires the __init_.hpp header to be included after this class inclusion.
 */
class Leds {
    private:
        static void initialize();
		Leds();

    public:
        static bool __initialized;
        
		/**
		 * @brief Turns all given Leds on.
		 */
		static void turn_on(std::initializer_list<Led> leds);
		/**
		 * @brief Turns all given Leds off.
		 */
		static void turn_off(std::initializer_list<Led> leds);
};
#endif // LEDS_HPP