/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SERIAL_STM32SERIAL_HPP
#define LIBSMART_STM32SERIAL_STM32SERIAL_HPP

#include <libsmart_config.hpp>
#include <cstdint>
#include "Loggable.hpp"
#include "StreamSession/Manager.hpp"
#include "StreamSession/NullStreamSession.hpp"
#include "StreamSession/StreamSessionAware.hpp"

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
    class Stm32Serial
            : public Stm32Common::Process::ProcessInterface,
              public Stm32Common::StreamSession::StreamSessionAware,
              public Stm32ItmLogger::Loggable,
              public Stm32Common::Nameable,
              public Stm32Common::Stream {
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
        explicit Stm32Serial(AbstractDriver *driver)
            : Stm32Serial(driver, nullptr, &Stm32ItmLogger::emptyLogger) { ; }


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
        Stm32Serial(AbstractDriver *driver, Stm32ItmLogger::LoggerInterface *logger)
            : Stm32Serial(driver, nullptr, logger) { ; }

        Stm32Serial(AbstractDriver *driver, Stm32Common::StreamSession::ManagerInterface *session_mgr)
            : Stm32Serial(driver, session_mgr, &Stm32ItmLogger::emptyLogger) { ; }

        Stm32Serial(
            AbstractDriver *driver,
            Stm32Common::StreamSession::ManagerInterface *session_mgr,
            Stm32ItmLogger::LoggerInterface *logger);

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
         * @brief Initializes the Stm32Serial object.
         *
         * This function is responsible for setting up the Stm32Serial object by invoking the `begin` method.
         * It is typically called to initialize the serial communication and prepare the interface for data transmission and reception.
         */
        void setup() override;


        /**
         * @brief End the serial communication interface.
         *
         * This function ends the serial communication interface by calling the `end` function of the associated `driver`.
         * It should be called when you are done using the serial communication interface.
         */
        void end() override;


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
        void loop() override;


        Stm32Common::StreamSession::StreamSessionInterface *getSession() {
            // Return nullStreamSession, if component is not running
            if (!isRunning) return &Stm32Common::StreamSession::nullStreamSession;

            // Return nullStreamSession, if no session manager is present
            if (!hasSessionManager()) return &Stm32Common::StreamSession::nullStreamSession;

            // Create a (hopefully) unique session id
            sessionId = sessionId != 0 ? sessionId : reinterpret_cast<uint32_t>(this);

            // Get the session, if it exists
            auto session = getSessionManager()->getSessionById(sessionId);

            if (session == nullptr) {
                if (isInIsr()) {
                    // Don't start a new session, if in isr
                    return &Stm32Common::StreamSession::nullStreamSession;
                }

                // Create a new session
                session = getSessionManager()->getNewSession(sessionId);
                if (session == nullptr) {
                    // Still no session => error
                    isRunning = false;
                    log()->setSeverity(Stm32ItmLogger::LoggerInterface::Severity::ERROR)
                            ->printf("Can not start session (%s)\r\n", getName());
                    end();
                    return &Stm32Common::StreamSession::nullStreamSession;
                }

                // Initialize session
                session->setName(getName());
                session->setLogger(getLogger());
                session->setup();
            }

            return session;
        }

        auto *getRxBuffer() { return getSession()->getRxBuffer(); }

        auto *getTxBuffer() { return getSession()->getTxBuffer(); }

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
        size_t getWriteBuffer(uint8_t *&buffer) override { return getSession()->getWriteBuffer(buffer); }

        size_t setWrittenBytes(size_t size) override { return getSession()->setWrittenBytes(size); }
#endif

        size_t write(uint8_t data) override;

        using Stream::write;

        int availableForWrite() override { return getSession()->availableForWrite(); }

        void flush() override;

        int available() override { return getSession()->available(); }

        int read() override { return getSession()->read(); }

        int peek() override { return getSession()->peek(); }

    private:
        AbstractDriver *driver;
        uint32_t sessionId{};
        bool isRunning = false;
    };
}

#endif //LIBSMART_STM32SERIAL_STM32SERIAL_HPP
