/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SERIAL_STM32SERIAL_HPP
#define LIBSMART_STM32SERIAL_STM32SERIAL_HPP

#include <cstdint>
#include "main.hpp"
#include "AbstractDriver.hpp"
#include "usart.h"
#include "StringBuffer.hpp"

namespace Stm32Serial {

    class Stm32Serial {
        friend AbstractDriver;

    public:
        explicit Stm32Serial(AbstractDriver *driver) : driver(driver) { driver->setSerialInstance(this); }

        void begin() { driver->begin(); };

        void end() { driver->end(); };

        void loop() {

            if(rxBuffer.getLength() > 0) {
//                Debugger_log(DBG, "Received: '%.*s'", rxBuffer.getLength(), rxBuffer.getStart());
//                printf("Received: '%.*s'", rxBuffer.getLength(), rxBuffer.getStart());
                printf("Received: %d", rxBuffer.getLength());
            }
        }

        typedef Stm32Common::StringBuffer<256> txBuffer_t;
        typedef Stm32Common::StringBuffer<256> rxBuffer_t;
        rxBuffer_t *getRxBuffer() { return &rxBuffer; }

    private:
        AbstractDriver *driver;
        rxBuffer_t rxBuffer;
        txBuffer_t txBuffer;
    };

}

#endif //LIBSMART_STM32SERIAL_STM32SERIAL_HPP
