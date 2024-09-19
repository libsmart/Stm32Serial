/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <libsmart_config.hpp>
#ifdef LIBSMART_STM32SERIAL_ENABLE_HAL_UART_IT_DRIVER

#include "Stm32HalUartItDriver.hpp"
#include "EmptyLogger.hpp"
#include "globals.hpp"
#include "Helper.hpp"

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    auto obj = Stm32Serial::AbstractDriver::findInRegistryByUniqueId((uint32_t) &huart->Instance);
    if (obj != nullptr) {
#ifdef __GXX_RTTI
        auto *driver = dynamic_cast<Stm32Serial::Stm32HalUartItDriver *>(obj);
#else
        auto *driver = static_cast<Stm32Serial::Stm32HalUartItDriver *>(obj);
#endif
        if (driver != nullptr) {
            driver->_txIsr();
        }
    }
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    auto obj = Stm32Serial::AbstractDriver::findInRegistryByUniqueId((uint32_t) &huart->Instance);
    if (obj != nullptr) {
#ifdef __GXX_RTTI
        auto *driver = dynamic_cast<Stm32Serial::Stm32HalUartItDriver *>(obj);
#else
        auto *driver = static_cast<Stm32Serial::Stm32HalUartItDriver *>(obj);
#endif
        if (driver != nullptr) {
            driver->_rxIsr(Size);
        }
    }
}


void Stm32Serial::Stm32HalUartItDriver::begin(unsigned long baud, uint8_t config) {
    log()->println("Stm32Serial::Stm32HalUartItDriver::begin()");

    AbstractDriver::begin(baud, config);
    auto ret = HAL_UARTEx_ReceiveToIdle_IT(huart, rx_buff, sizeof rx_buff);
    if (ret != HAL_OK) {
        log()->print("HAL_UARTEx_ReceiveToIdle_IT = 0x");
        log()->println(ret, HEX);
    }
}


void Stm32Serial::Stm32HalUartItDriver::_rxIsr(uint16_t Size) {
    getRxBuffer()->write(rx_buff, Size);
    // getTxBuffer()->write(rx_buff, Size);
    memset(rx_buff, 0, sizeof rx_buff);
    HAL_UARTEx_ReceiveToIdle_IT(huart, rx_buff, sizeof rx_buff);
}


void Stm32Serial::Stm32HalUartItDriver::_txIsr() {
    auto txBuffer = getTxBuffer();
#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_READ
    if (txBuffer->getLength() > 0) {
        auto ret = this->transmit(txBuffer->getReadPointer(), txBuffer->getLength());
        if (ret > 0) {
            txBuffer->remove(ret);
        }
    }
#else
    if (txBuffer->getLength() > 0) {
        uint8_t ch = txBuffer->peek();
        if(this->transmit(&ch,1) == 1) {
            txBuffer->remove(1);
        }
    }
#endif
}


void Stm32Serial::Stm32HalUartItDriver::loop() {
    AbstractDriver::loop();
}


void Stm32Serial::Stm32HalUartItDriver::checkTxBufferAndSend() {
    _txIsr();
}


#endif
