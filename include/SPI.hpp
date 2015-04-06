#ifndef SPI_HPP
#define SPI_HPP

#include <string>

#include "stm32f4xx.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_spi.h"

/**
 * Implements basic operations on SPI bus
 */
class SPI {
    private:
        static void initialize();
        SPI();

    public:
        static bool initialized;
        static bool master;
        static uint8_t channel;

        /**
         * Changes SPI priority to master
         * @param bool True if master, false if slave
         */
        static void setMaster(bool);

        /**
         * Changes SPI output channel
         * @param uint8_t Channel number
         */
        static void setChannel(uint8_t);

        /**
         * Sends data via SPI
         * @param std::string Data to send
         */
        static void sendData(std::string);
};

#endif  