/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <libsmart_config.hpp>
#include "Stm32HalUartItDriver.hpp"
#include "globals.hpp"

#ifdef LIBSMART_STM32SERIAL_ENABLE_HAL_UART_IT_DRIVER


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



void Stm32Serial::Stm32HalUartItDriver::begin(unsigned long baud, uint8_t config) {
    AbstractDriver::begin(baud, config);
    //            HAL_UART_MspInit(huart);
    //            HAL_UART_Init(huart);
    HAL_UARTEx_ReceiveToIdle_IT(huart, rx_buff, sizeof rx_buff);

    //            HAL_UART_Receive_IT(huart, rx_buff, sizeof rx_buff);
}




void Stm32Serial::Stm32HalUartItDriver::rxIsr(uint16_t Size) {
    getRxBuffer()->write(rx_buff, Size);
    getTxBuffer()->write(rx_buff, Size);
    memset(rx_buff, 0, sizeof rx_buff);
    HAL_UARTEx_ReceiveToIdle_IT(huart, rx_buff, sizeof rx_buff);
}

void Stm32Serial::Stm32HalUartItDriver::txIsr() {
    auto txBuffer = getTxBuffer();
    if (txBuffer->getLength() > 0) {
        auto ret = this->transmit(txBuffer->getReadPointer(), txBuffer->getLength());
        if (ret > 0) {
            txBuffer->remove(ret);
        }
    }
}


#endif
