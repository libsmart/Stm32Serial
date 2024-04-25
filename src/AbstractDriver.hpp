/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SERIAL_ABSTRACTDRIVER_HPP
#define LIBSMART_STM32SERIAL_ABSTRACTDRIVER_HPP

#include "Stm32Serial.hpp"

namespace Stm32Serial {

    class AbstractDriver {
        friend class Stm32Serial;

    public:
        AbstractDriver(Stm32Serial *ser, const char *name, uint32_t uniqueId) : ser(ser), name(name),
                                                                                uniqueId(uniqueId) {
            registerDriver();
        }

        AbstractDriver(Stm32Serial *ser, uint32_t uniqueId) : AbstractDriver(ser, nullptr, uniqueId) {}

        explicit AbstractDriver(const char *name) : AbstractDriver(nullptr, name, 0) {}

        explicit AbstractDriver(uint32_t uniqueId) : AbstractDriver(nullptr, nullptr, uniqueId) {}

        AbstractDriver(const char *name, uint32_t uniqueId) : AbstractDriver(nullptr, name, uniqueId) {}

        /**
         * @brief Initializes the serial communication with the specified baud rate and configuration.
         *
         * This function should be implemented by a derived class.
         *
         * @param baud - The baud rate for the serial communication.
         * @param config - The configuration for the serial communication.
         */
        virtual void begin(unsigned long baud, uint8_t config) {}


        /**
         * @brief Ends the serial communication.
         *
         * This function should be implemented by a derived class.
         * It is responsible for stopping the serial communication.
         */
        virtual void end() {}


        /**
         * @brief Flushes the serial communication buffer.
         */
        virtual void flush() {}


        /**
         * @brief Checks if the serial port is currently connected.
         * @return true if the driver is connected, false otherwise.
         */
        virtual bool isConnected() { return true; }


        const char *getName() { return name; }

        uint32_t getUniqueId() const { return uniqueId; }

        static AbstractDriver *findInRegistryByName(const char *name) {
            for (auto item: registry) {
                if (strcmp(item->getName(), name) == 0) return item;
            }
            return nullptr;
        }

        static AbstractDriver *findInRegistryByUniqueId(uint32_t uniqueId) {
            for (auto item: registry) {
                if (item->getUniqueId() == uniqueId) return item;
            }
            return nullptr;
        }


    protected:
        /**
         * @brief Set the serial instance for the Stm32Serial object.
         *
         * This function sets the serial instance for the Stm32Serial object.
         * It associates the provided Stm32Serial object with the specified serial instance.
         *
         * @param serial The Stm32Serial instance to be associated with the serial instance.
         */
        virtual void setSerialInstance(Stm32Serial *serial) { ser = serial; }


        /**
         * @brief Transmit a null-terminated string.
         * @param str - The null-terminated string to transmit.
         */
        virtual void transmit(const char *str) { transmit(reinterpret_cast<const uint8_t *>(str), strlen(str)); }


        /**
         * @brief Transmit a string of bytes.
         *
         * This function is a virtual function that should be implemented by a derived class.
         * It transmits the specified string with the given length.
         *
         * @param str - The string of bytes to transmit.
         * @param strlen - The length of the string.
         *
         * @return The number of bytes successfully transmitted.
         */
        virtual size_t transmit(const uint8_t *str, size_t strlen) { return 0; }


        /**
          * @brief The checkTxBufferAndSend function checks the transmit buffer and sends any pending data.
          *
          * This function is a virtual function that should be implemented by a derived class.
          * It checks if there is any data in the transmit buffer and sends it over the serial communication.
          * If there is no data in the buffer, this function does nothing.
          */
        virtual void checkTxBufferAndSend() {}


        /**
         * @brief Get the receive buffer of the serial communication.
         *
         * This function returns a pointer to the receive buffer of the serial communication.
         *
         * @return A pointer to the receive buffer.
         */
        virtual Stm32Serial::rxBuffer_t *getRxBuffer() { return &ser->rxBuffer; }


        /**
         * @brief Get the transmit buffer of the serial communication.
         *
         * This function returns a pointer to the transmit buffer of the serial communication.
         * The transmit buffer is used to store data that needs to be sent over the serial communication.
         *
         * @return A pointer to the transmit buffer.
         */
        virtual Stm32Serial::txBuffer_t *getTxBuffer() { return &ser->txBuffer; }


    protected:

        /**
         * @brief Registers the current driver in the registry.
         *
         * This function registers the current driver instance in the static registry array.
         * It searches for an empty slot in the registry and stores the current driver instance in it.
         * If no empty slot is found, the function does nothing.
         */
        void registerDriver() {
            for (size_t i = 0; i < (sizeof registry / sizeof registry[0]); i++) {
                if (registry[i] == nullptr) {
                    registry[i] = this;
                    return;
                }
            }
        }


        /** Pointer to the serial object */
        Stm32Serial *ser = {};

        /** Name of the object */
        const char *name = {};

        uint32_t uniqueId = {};

        static AbstractDriver *registry[5];

    };

}

#endif //LIBSMART_STM32SERIAL_ABSTRACTDRIVER_HPP
