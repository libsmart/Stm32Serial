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


#include "AbstractDriver.hpp"
#include "Stm32Serial.hpp"

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
        Stm32HalUartItDriver(UART_HandleTypeDef *huart, const char *name) : huart(huart),
                                                                            AbstractDriver(
                                                                                name, (uint32_t) &huart->Instance) {
        };

        Stm32HalUartItDriver(UART_HandleTypeDef *huart, uint32_t uniqueId) : huart(huart), AbstractDriver(uniqueId) {
        };

        explicit Stm32HalUartItDriver(UART_HandleTypeDef *huart) : huart(huart),
                                                                   AbstractDriver((uint32_t) &huart->Instance) {
        };

        void begin(unsigned long baud, uint8_t config) override;

        void rxIsr(uint16_t Size);

        void txIsr();

    protected:
        size_t transmit(const uint8_t *str, size_t strlen) override {
            if(huart->gState != HAL_UART_STATE_READY) {
                return 0;
            }
            size_t sz = strlen > sizeof tx_buff ? sizeof tx_buff : strlen;
            memset(tx_buff, 0, sizeof tx_buff);
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
