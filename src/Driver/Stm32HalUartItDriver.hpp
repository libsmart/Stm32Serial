/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.hpp"

#ifdef __cplusplus
extern "C" {
#endif
void Stm32HalUartItDriver_isr(UART_HandleTypeDef *huart);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus


#ifndef LIBSMART_STM32SERIAL_STM32HALUARTITDRIVER_HPP
#define LIBSMART_STM32SERIAL_STM32HALUARTITDRIVER_HPP


#include "src/AbstractDriver.hpp"
#include "src/Stm32Serial.hpp"

/**
 *
 * Add Stm32HalUartItDriver_isr to stm32f4xx_it.c function USART3_IRQHandler().
 *
 */
namespace Stm32Serial {

    class Stm32HalUartItDriver : public AbstractDriver {
        friend Stm32Serial;

    public:
        Stm32HalUartItDriver(UART_HandleTypeDef *huart) : huart(huart) {};

        void begin() override {
            AbstractDriver::begin();
//            HAL_UART_MspInit(huart);
//            HAL_UART_Init(huart);
            HAL_UARTEx_ReceiveToIdle_IT(huart, rx_buff, sizeof rx_buff);

//            HAL_UART_Receive_IT(huart, rx_buff, sizeof rx_buff);

        }

        void isr(uint16_t Size) {
//            Debugger_log(DBG, "Stm32Serial::isr(%d)", Size);
            ser->getRxBuffer()->write(rx_buff, Size);
            HAL_UARTEx_ReceiveToIdle_IT(huart, rx_buff, sizeof rx_buff);
        }


    private:
        UART_HandleTypeDef *huart;
        uint8_t tx_buff[16];
        uint8_t rx_buff[16];

    };

}

#endif //LIBSMART_STM32SERIAL_STM32HALUARTITDRIVER_HPP
#endif
