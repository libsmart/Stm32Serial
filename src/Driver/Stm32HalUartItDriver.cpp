/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Stm32HalUartItDriver.hpp"
#include "globals.hpp"

/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        Debugger_log(DBG, "HAL_UART_RxCpltCallback()");
    }
}
 */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    auto obj = Stm32Serial::AbstractDriver::findInRegistryByUniqueId((uint32_t) &huart->Instance);
    if (obj != nullptr) {
        auto *driver = dynamic_cast<Stm32Serial::Stm32HalUartItDriver *>(obj);
        if(driver != nullptr) {
            driver->txIsr();
        }
    }
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    auto obj = Stm32Serial::AbstractDriver::findInRegistryByUniqueId((uint32_t) &huart->Instance);
    if (obj != nullptr) {
        auto *driver = dynamic_cast<Stm32Serial::Stm32HalUartItDriver *>(obj);
        if(driver != nullptr) {
            driver->rxIsr(Size);
        }
    }
}

