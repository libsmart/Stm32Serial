/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SERIAL_STM32SERIAL_HPP
#define LIBSMART_STM32SERIAL_STM32SERIAL_HPP

#include <cstdint>
#include <climits>
#include "main.hpp"
#include "usart.h"
#include "StringBuffer.hpp"
#include "Stream.hpp"


#define DEFAULT_BAUD 115200
#define DEFAULT_CONFIG 0
#define BUFFER_SIZE_RX 256
#define BUFFER_SIZE_TX 256


namespace Stm32Serial {
    class AbstractDriver;

    /**
     * @class Stm32Serial
     * @brief A class representing a serial communication interface on the STM32 platform.
     * @details This class provides methods to initialize, configure, and use the serial port for communication.
     */
    class Stm32Serial : public Stm32Common::Stream {
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
        explicit Stm32Serial(AbstractDriver *driver);


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
         * @brief Get the number of bytes (characters) available for reading from the serial port.
         *
         * This is data that’s already arrived and stored in the serial receive buffer (which holds BUFFER_SIZE_RX bytes).
         *
         * @return number of bytes available for reading
         */
        int available() override;


        /**
         * @brief Get the next byte (character) from the serial port without removing it from the buffer.
         *
         * @return int The next byte (character) to be read from the serial port, or -1 if no data is available.
         */
        int peek() override;


        /**
         * @brief Read a byte from the serial port.
         *
         * @return The byte read from the serial port, or -1 if no data is available.
         */
        int read() override;


        /**
         * @brief Get the number of bytes (characters) available for writing in the serial buffer without blocking the
         * write operation.
         *
         * @return The number of bytes available for writing to the serial port.
         */
        int availableForWrite() override;


        /**
         * @brief Waits for the transmission of outgoing serial data to complete.
         */
        void flush() override;


        /**
         * @brief Writes a byte to the serial port.
         *
         * @param uint8 The byte to be written to the serial port.
         * @return The number of bytes written.
         */
        size_t write(uint8_t uint8) override;

        // Make other write() methods visible.
        using Print::write;


        /**
         * @brief Return true, if serial port is connected.
         */
        operator bool();


        /**
         * @brief Get the write buffer for direct write override.
         *
         * This function returns a pointer to the write buffer and the size of the available space in the buffer.
         * The write buffer is used for direct write override, allowing data to be written directly to the buffer
         * without copying it. The caller can use this buffer to write data directly to the serial port.
         *
         * @param[out] buffer A reference to a pointer that will hold the write buffer.
         * @return The size of the available space in the write buffer.
         */
        size_t getWriteBuffer(uint8_t *&buffer) DIRECT_BUFFER_WRITE_OVERRIDE;


        /**
         * @brief Sets the number of written bytes in the write buffer.
         *
         * This function sets the number of bytes that have been written to the serial port and updates the write buffer accordingly.
         *
         * @param size The number of bytes written.
         * @return The updated size of the write buffer.
         */
        size_t setWrittenBytes(size_t size) DIRECT_BUFFER_WRITE_OVERRIDE;


        /**
         * @fn void loop()
         * @brief Main loop function for the serial communication.
         * @details This function is called in a loop to handle incoming and outgoing data in the serial communication.
         * It checks if there is any data received in the rxBuffer and logs it using the Debugger_log function.
         * It then clears the rxBuffer. Finally, it calls the checkTxBufferAndSend function of the driver to transmit any pending data in the txBuffer.
         */
        void loop();


        typedef Stm32Common::StringBuffer<BUFFER_SIZE_TX> txBuffer_t;
        typedef Stm32Common::StringBuffer<BUFFER_SIZE_RX> rxBuffer_t;

    private:
        AbstractDriver *driver;
        rxBuffer_t rxBuffer;
        // txBuffer_t txBuffer;

        class txBufferClass final : public txBuffer_t {
        public:
            txBufferClass() = delete;

            explicit txBufferClass(Stm32Serial &self)
                : self(self) {
            }

        protected:
            void onWrite() override;

            Stm32Serial &self;
        } txBuffer{*this};
    };
}

#endif //LIBSMART_STM32SERIAL_STM32SERIAL_HPP
