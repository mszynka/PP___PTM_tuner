#include "error_handlers.hpp"

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
    Leds::turn_on({Led::Red});
    for(int i=0; i<= 100000; i++);
    Leds::turn_off({Led::Red});
    for(int i=0; i<= 100000; i++);
  }
}

/**
  * @brief  This function is executed in case of error occurrence with output message.
  * @param  error_message
  * @retval None
  */
static void Error_Handler(error_message)
{
  Leds::turn_on({error_message});
  Error_Handler();
}