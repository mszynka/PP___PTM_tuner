/**
 * LEGACY CODE
 */

#include "error_handlers.hpp"

void Error_Handler(void)
{
  while(1)
  {
    Leds::turn_on({Led::Red});
    for(int i=0; i<= 100000; i++);
    Leds::turn_off({Led::Red});
    for(int i=0; i<= 100000; i++);
  }
}

void mError_Handler(int8_t value)
{
  Leds::reset();
  switch(value)
  {
    case 0:
      Leds::turn_on({Led::Green});
      break;
    case 1:
      Leds::turn_on({Led::Orange});
      break;
    case 2:
      Leds::turn_on({Led::Blue});
      break;
    default:
      Leds::turn_on({Led::Green,Led::Orange,Led::Blue});
      break;
  }
  Error_Handler();
}