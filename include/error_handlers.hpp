/**
 * LEGACY CODE
 */

#ifndef ERROR_HANDLERS_HPP
#define ERROR_HANDLERS_HPP

#include "Leds.hpp"

/**
 * 	@brief	Contains convenient names for error messages.
 * 	       	Color-coding-wise:
 * 	       		Green	task
 * 	       		Yellow	audio
 * 	       		Blue	display
 */
enum class error_message : uint16_t {
	task = Led::Green,
	audio = Led::Yellow,
	display = Led::Blue
};

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void);

/**
  * @brief  This function is executed in case of error occurrence with output message.
  * @param  error_message
  * @retval None
  */
static void Error_Handler(error_message);

#endif