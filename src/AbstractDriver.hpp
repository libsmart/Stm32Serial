/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SERIAL_ABSTRACTDRIVER_HPP
#define LIBSMART_STM32SERIAL_ABSTRACTDRIVER_HPP

namespace Stm32Serial {

    class Stm32Serial;

    class AbstractDriver {
        friend Stm32Serial;

    public:

        virtual void begin() {};

        virtual void end() {};

    protected:
        virtual void setSerialInstance(Stm32Serial *serial) { ser = serial; };

    protected:
        Stm32Serial *ser{};

    };

}

#endif //LIBSMART_STM32SERIAL_ABSTRACTDRIVER_HPP
