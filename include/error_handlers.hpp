/**
 * LEGACY CODE
 */

#ifndef ERROR_HANDLERS_HPP
#define ERROR_HANDLERS_HPP

#include "Leds.hpp"

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void);

/**
  * @brief  This function is executed in case of error occurrence with output message.
  * 	      Color-coding-wise:
 	* 	      	Green		task 		 0
 	* 	      	Orange	audio		 1
 	* 	      	Blue		display  2
	* @param  uint8_t
	* @retval None
	*/
void mError_Handler(uint8_t value);


#endif // ERROR_HANDLERS_HPP