/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "AbstractDriver.hpp"

namespace Stm32Serial {
    class Stm32HalUartDmaDriver : public AbstractDriver {
        friend class Stm32Serial;

    public:
        Stm32HalUartDmaDriver(UART_HandleTypeDef *huart)
                : AbstractDriver(reinterpret_cast<uint32_t>(&huart->Instance)), huart(huart) { ; }

        Stm32HalUartDmaDriver(UART_HandleTypeDef *huart, const char *name)
                : AbstractDriver(name, reinterpret_cast<uint32_t>(&huart->Instance)), huart(huart) { ; }

        Stm32HalUartDmaDriver(UART_HandleTypeDef *huart, const uint32_t uniqueId)
                : AbstractDriver(uniqueId), huart(huart) { ; }


    protected:
        size_t transmit(const uint8_t *str, size_t strlen) override;

    private:
        /**
         * @brief Pointer to an instance of the UART_HandleTypeDef structure.
         *
         * This variable is used to store the handle of the UART peripheral. The UART_HandleTypeDef structure is a HAL
         * UART handle containing various configuration and state information for the UART.
         */
        UART_HandleTypeDef *huart;
    };
}
