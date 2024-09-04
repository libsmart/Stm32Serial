/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SERIAL_STM32SERIAL_HPP
#define LIBSMART_STM32SERIAL_STM32SERIAL_HPP

#include <libsmart_config.hpp>
#include <cstdint>
#include <climits>
#include "StringBuffer.hpp"
#include "Loggable.hpp"
#include "StreamRxTx.hpp"

#define DEFAULT_BAUD 115200
#define DEFAULT_CONFIG 0


namespace Stm32Serial {
    class AbstractDriver;

    /**
     * Stm32Serial constructor.
     *
     * Constructs a `Stm32Serial` object with an `AbstractDriver` instance as a parameter. It sets the `driver` member
     * variable to the provided driver and establishes the bidirectional association between the `driver` and the
     * `Stm32Serial` instance.
     *
     * @param driver The `AbstractDriver` object that will be associated with the `Stm32Serial` instance.
     */
    class Stm32Serial : public Stm32ItmLogger::Loggable,
                        public Stm32Common::StreamRxTx<LIBSMART_STM32SERIAL_BUFFER_SIZE_RX,
                            LIBSMART_STM32SERIAL_BUFFER_SIZE_TX> {
        friend class AbstractDriver;

    public:
        /**
         * @brief Stm32Serial constructor
         *
         * Constructs a `Stm32Serial` object with an `AbstractDriver` instance as a parameter. It sets the `driver` member
         * variable to the provided driver and establishes the bidirectional association between the `driver` and the
         * `Stm32Serial` instance.
         *
         * @param driver The `AbstractDriver` object that will be associated with the `Stm32Serial` instance.
         */
        explicit Stm32Serial(AbstractDriver *driver): Stm32Serial(driver, &Stm32ItmLogger::emptyLogger) { ; }


        /**
         * @brief Stm32Serial constructor.
         *
         * Constructs a `Stm32Serial` object with an `AbstractDriver` instance and a `LoggerInterface` instance as
         * parameters. It sets the `driver` member variable to the provided driver and establishes the bidirectional
         * association between the `driver` and the `Stm32Serial` instance. Additionally, it sets the `logger` member
         * variable to the provided logger.
         *
         * @param driver The `AbstractDriver` object that will be associated with the `Stm32Serial` instance.
         * @param logger The `LoggerInterface` object that will be associated with the `Stm32Serial` instance.
         */
        Stm32Serial(AbstractDriver *driver, Stm32ItmLogger::LoggerInterface *logger);


        /**
         * @brief Initialize the serial communication interface with the specified baud rate and configuration.
         *
         * This function initializes the serial port. It configures the baud rate and other communication settings.
         *
         * @param baud The baud rate for the serial communication.
         * @param config The configuration for the serial communication.
         */
        void begin(unsigned long baud, uint8_t config);


        /**
         * @brief Initialize the serial communication interface with the specified baud rate and default configuration.
         *
         * This function initializes the serial port. It configures the baud rate and other communication settings.
         *
         * @param baud The baud rate for the serial communication.
         */
        void begin(unsigned long baud);


        /**
         * @brief Initialize the serial communication interface with the default baud rate and default configuration.
         *
         * This function initializes the serial port. It configures the baud rate and other communication settings.
         */
        void begin();


        /**
         * @brief End the serial communication interface.
         *
         * This function ends the serial communication interface by calling the `end` function of the associated `driver`.
         * It should be called when you are done using the serial communication interface.
         */
        void end();


        /**
         * @brief Return true, if serial port is connected.
         */
        operator bool();


        /**
         * @fn void loop()
         * @brief Main loop function for the serial communication.
         * @details This function is called in a loop to handle incoming and outgoing data in the serial communication.
         * It checks if there is any data received in the rxBuffer and logs it using the Debugger_log function.
         * It then clears the rxBuffer. Finally, it calls the checkTxBufferAndSend function of the driver to transmit any pending data in the txBuffer.
         */
        virtual void loop();


    private:
        AbstractDriver *driver;
    };
}

#endif //LIBSMART_STM32SERIAL_STM32SERIAL_HPP
