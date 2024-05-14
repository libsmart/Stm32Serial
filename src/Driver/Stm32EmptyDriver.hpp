/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef LIBSMART_STM32SERIAL_STM32EMPTYDRIVER_HPP
#define LIBSMART_STM32SERIAL_STM32EMPTYDRIVER_HPP

#include <libsmart_config.hpp>
#include "AbstractDriver.hpp"

namespace Stm32Serial {
    class Stm32EmptyDriver : public AbstractDriver {
    public:
        explicit Stm32EmptyDriver(const char *name) : AbstractDriver(name) {}

    protected:
        size_t transmit(const uint8_t *str, size_t strlen) override;
    };
}

#endif //LIBSMART_STM32SERIAL_STM32EMPTYDRIVER_HPP
