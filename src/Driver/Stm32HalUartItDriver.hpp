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
        friend class Stm32Serial;

    public:
//        Stm32HalUartItDriver(UART_HandleTypeDef *huart) : huart(huart) {};
        Stm32HalUartItDriver(UART_HandleTypeDef *huart, const char *name) : huart(huart), AbstractDriver(name, (uint32_t)&huart->Instance) {};
        Stm32HalUartItDriver(UART_HandleTypeDef *huart, uint32_t uniqueId) : huart(huart), AbstractDriver(uniqueId) {};
        explicit Stm32HalUartItDriver(UART_HandleTypeDef *huart) : huart(huart), AbstractDriver((uint32_t)&huart->Instance) {};

        void begin(unsigned long baud, uint8_t config) override {
            AbstractDriver::begin(baud, config);
//            HAL_UART_MspInit(huart);
//            HAL_UART_Init(huart);
            HAL_UARTEx_ReceiveToIdle_IT(huart, rx_buff, sizeof rx_buff);

//            HAL_UART_Receive_IT(huart, rx_buff, sizeof rx_buff);

        }

        void rxIsr(uint16_t Size) {
            getRxBuffer()->write(rx_buff, Size);
            getTxBuffer()->write(rx_buff, Size);
            HAL_UARTEx_ReceiveToIdle_IT(huart, rx_buff, sizeof rx_buff);
        }

        void txIsr() {
            auto txBuffer = getTxBuffer();
            if (txBuffer->getLength() > 0) {
                auto ret = this->transmit(txBuffer->getStart(), txBuffer->getLength());
                if (ret > 0) {
                    txBuffer->remove(ret);
                }
            }
        }

    protected:
        size_t transmit(const uint8_t *str, size_t strlen) override {
            size_t sz = strlen > sizeof tx_buff ? sizeof tx_buff : strlen;
            memcpy(tx_buff, str, sz);
            if (HAL_OK == HAL_UART_Transmit_IT(huart, tx_buff, sz)) {
                return sz;
            }
            return 0;
        }

        void checkTxBufferAndSend() override {
            txIsr();
        }


    private:
        UART_HandleTypeDef *huart;
        uint8_t tx_buff[32] = {};
        uint8_t rx_buff[32] = {};
    };

}

#endif //LIBSMART_STM32SERIAL_STM32HALUARTITDRIVER_HPP
#endif
