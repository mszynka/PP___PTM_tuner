#ifndef LCD_HPP
#define LCD_HPP

#include "stm32f4xx.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_i2c.h"

#include "error_handlers.hpp"

#define RXBUFFERSIZE 32
#define TXBUFFERSIZE 32

/* Buffer used for reception */
//uint8_t aRxBuffer[RXBUFFERSIZE];
//uint16_t hTxNumData = 0, hRxNumData = 0;
//uint8_t bTransferRequest = 0;

// Transmission buffer TMP
//uint8_t aTxBuffer[] = " ****I2C_TwoBoards advanced communication based on IT****  ****I2C_TwoBoards advanced communication based on IT****  ****I2C_TwoBoards advanced communication based on IT**** ";

class LCD {
    private:
        static void initialize();
        LCD();

    public:
        static bool __initialized;

        static void sendData();
};

#endif