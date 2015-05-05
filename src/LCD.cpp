#include "LCD.hpp"

#include "error_handlers.hpp"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_i2c.h"

#define I2C_ADDRESS        0x3E
#define MASTER_REQ_READ    0x12
#define MASTER_REQ_WRITE   0x34

I2C_HandleTypeDef I2CxHandle;

void LCD::initialize(){
  I2CxHandle.Instance             = I2C1;
  I2CxHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2CxHandle.Init.ClockSpeed      = 400000;
  I2CxHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2CxHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
  I2CxHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2CxHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  I2CxHandle.Init.OwnAddress1     = I2C_ADDRESS;  
  I2CxHandle.Init.OwnAddress2     = 0;

  if(HAL_I2C_Init(&I2CxHandle) != HAL_OK)
  {
    /* Initialization Error */
    mError_Handler(2);
  }
}

void LCD::sendData(){
     /* Initialize number of data variables */
    uint16_t hTxNumData = TXBUFFERSIZE;
    uint16_t hRxNumData = RXBUFFERSIZE;
    uint8_t aRxBuffer[RXBUFFERSIZE];

    uint8_t aTxBuffer[] = " ****I2C_TwoBoards advanced communication based on IT****  ****I2C_TwoBoards advanced communication based on IT****  ****I2C_TwoBoards advanced communication based on IT**** ";

    /* Update bTransferRequest to send buffer write request for Slave */
    uint8_t bTransferRequest = MASTER_REQ_WRITE;

    /*##-2- Master sends write request for slave #############################*/
    while(HAL_I2C_Master_Transmit_IT(&I2CxHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)&bTransferRequest, 1)!= HAL_OK)
    {
      /* Error_Handler() function is called when Timeout error occurs.
         When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
      if (HAL_I2C_GetError(&I2CxHandle) != HAL_I2C_ERROR_AF)
      {
        mError_Handler(2);
      }
    }

    /*  Before starting a new communication transfer, you need to check the current
    state of the peripheral; if its busy you need to wait for the end of current
    transfer before starting a new one.
    For simplicity reasons, this example is just waiting till the end of the
    transfer, but application may perform other tasks while transfer operation
    is ongoing. */
    while (HAL_I2C_GetState(&I2CxHandle) != HAL_I2C_STATE_READY)
    {
    }

    /*##-3- Master sends number of data to be written ########################*/
    while(HAL_I2C_Master_Transmit_IT(&I2CxHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)&hTxNumData, 2)!= HAL_OK)
    {
      /* Error_Handler() function is called when Timeout error occurs.
         When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
      if (HAL_I2C_GetError(&I2CxHandle) != HAL_I2C_ERROR_AF)
      {
        mError_Handler(2);
      }
    }

    /*  Before starting a new communication transfer, you need to check the current
    state of the peripheral; if its busy you need to wait for the end of current
    transfer before starting a new one.
    For simplicity reasons, this example is just waiting till the end of the
    transfer, but application may perform other tasks while transfer operation
    is ongoing. */
    while (HAL_I2C_GetState(&I2CxHandle) != HAL_I2C_STATE_READY)
    {
    }

    /*##-4- Master sends aTxBuffer to slave ##################################*/
    while(HAL_I2C_Master_Transmit_IT(&I2CxHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
    {
      /* Error_Handler() function is called when Timeout error occurs.
         When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
      if (HAL_I2C_GetError(&I2CxHandle) != HAL_I2C_ERROR_AF)
      {
        mError_Handler(2);
      }
    }

    /*  Before starting a new communication transfer, you need to check the current
    state of the peripheral; if its busy you need to wait for the end of current
    transfer before starting a new one.
    For simplicity reasons, this example is just waiting till the end of the
    transfer, but application may perform other tasks while transfer operation
    is ongoing. */
    while (HAL_I2C_GetState(&I2CxHandle) != HAL_I2C_STATE_READY)
    {
    }

    /* Update bTransferRequest to send buffer read request for Slave */
    bTransferRequest = MASTER_REQ_READ;

    /*##-5- Master sends read request for slave ##############################*/
    while(HAL_I2C_Master_Transmit_IT(&I2CxHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)&bTransferRequest, 1)!= HAL_OK)
    {
      /* Error_Handler() function is called when Timeout error occurs.
         When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
      if (HAL_I2C_GetError(&I2CxHandle) != HAL_I2C_ERROR_AF)
      {
        mError_Handler(2);
      }
    }

    /*  Before starting a new communication transfer, you need to check the current
    state of the peripheral; if its busy you need to wait for the end of current
    transfer before starting a new one.
    For simplicity reasons, this example is just waiting till the end of the
    transfer, but application may perform other tasks while transfer operation
    is ongoing. */
    while (HAL_I2C_GetState(&I2CxHandle) != HAL_I2C_STATE_READY)
    {
    }

    /*##-6- Master sends number of data to be read ###########################*/
    while(HAL_I2C_Master_Transmit_IT(&I2CxHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)&hRxNumData, 2)!= HAL_OK)
    {
      /* Error_Handler() function is called when Timeout error occurs.
         When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
      if (HAL_I2C_GetError(&I2CxHandle) != HAL_I2C_ERROR_AF)
      {
        mError_Handler(2);
      }
    }

    /*  Before starting a new communication transfer, you need to check the current
    state of the peripheral; if its busy you need to wait for the end of current
    transfer before starting a new one.
    For simplicity reasons, this example is just waiting till the end of the
    transfer, but application may perform other tasks while transfer operation
    is ongoing. */
    while (HAL_I2C_GetState(&I2CxHandle) != HAL_I2C_STATE_READY)
    {
    }

    /*##-7- Master receives aRxBuffer from slave #############################*/
    while(HAL_I2C_Master_Receive_IT(&I2CxHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)aRxBuffer, RXBUFFERSIZE)!= HAL_OK)
    {
      /* Error_Handler() function is called when Timeout error occurs.
         When Acknowledge failure occurs (Slave don't acknowledge it's address)
         Master restarts communication */
      if (HAL_I2C_GetError(&I2CxHandle) != HAL_I2C_ERROR_AF)
      {
        mError_Handler(2);
      }
    }

    /*  Before starting a new communication transfer, you need to check the current
    state of the peripheral; if its busy you need to wait for the end of current
    transfer before starting a new one.
    For simplicity reasons, this example is just waiting till the end of the
    transfer, but application may perform other tasks while transfer operation
    is ongoing. */
    while (HAL_I2C_GetState(&I2CxHandle) != HAL_I2C_STATE_READY)
    {
    }
}