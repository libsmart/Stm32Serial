/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Stm32HalUartDmaDriver.hpp"

size_t Stm32Serial::Stm32HalUartDmaDriver::transmit(const uint8_t *str, size_t strlen) {
    if (huart->gState != HAL_UART_STATE_READY) {
        return 0;
    }

    // HAL_UART_Transmit_DMA(huart, str, strlen);
    return 0;
}
