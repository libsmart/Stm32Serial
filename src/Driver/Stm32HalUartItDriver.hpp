/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.hpp"

#ifdef __cplusplus
extern "C" {
#endif
void Stm32HalUartItDriver_isr(UART_HandleTypeDef *huart);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus


#ifndef LIBSMART_STM32SERIAL_STM32HALUARTITDRIVER_HPP
#define LIBSMART_STM32SERIAL_STM32HALUARTITDRIVER_HPP


#include "AbstractDriver.hpp"
#include "Stm32Serial.hpp"

/**
 *
 * Add Stm32HalUartItDriver_isr to stm32f4xx_it.c function USART3_IRQHandler().
 *
 */
namespace Stm32Serial {
    class Stm32HalUartItDriver : public AbstractDriver {
        friend class Stm32Serial;

    public:
        Stm32HalUartItDriver(UART_HandleTypeDef *huart)
            : AbstractDriver(reinterpret_cast<uint32_t>(&huart->Instance)), huart(huart) { ; }

        Stm32HalUartItDriver(UART_HandleTypeDef *huart, const char *name)
            : AbstractDriver(name, reinterpret_cast<uint32_t>(&huart->Instance)), huart(huart) { ; }

        Stm32HalUartItDriver(UART_HandleTypeDef *huart, const uint32_t uniqueId)
            : AbstractDriver(uniqueId), huart(huart) { ; }


        /**
         * @brief Handle the RX interrupt service routine (ISR) for the Stm32HalUartItDriver class.
         *
         * This method is called by the RX interrupt callback function HAL_UARTEx_RxEventCallback. It handles the received data by writing it to the RX buffer and TX buffer, and then clears the RX buffer. It also calls the function HAL_UARTEx_ReceiveToIdle_IT to continue receiving data.
         *
         * @param Size The size of the received data.
         *
         * @note This method is called internally and should not be called directly.
         */
        void _rxIsr(uint16_t Size);


        /**
         * @brief Handle the TX interrupt service routine (ISR) for the Stm32HalUartItDriver class.
         *
         * This method is called by the TX interrupt callback function HAL_UART_TxCpltCallback. It handles the transmission of data by checking the TX buffer for data to be sent. If there is data in the TX buffer, it transmits data from the buffer using the `transmit` method and removes the transmitted data from the buffer.
         *
         * @note This method is called internally and should not be called directly.
         */
        void _txIsr();

    protected:
        /**
         * @brief Initializes the Stm32HalUartItDriver class and starts receiving data.
         *
         * This method sets up the UART communication by calling the `AbstractDriver::begin` method with the specified
         * baud rate and configuration. It also calls the function `HAL_UARTEx_ReceiveToIdle_IT` to enable the RX
         * interrupt and start receiving data. If an error occurs during the initialization, it logs the error message.
         *
         * @param baud The baud rate of the UART communication.
         * @param config The configuration of the UART communication.
         *
         * @note This method should be called before any data transmission or reception.
         */
        void begin(unsigned long baud, uint8_t config) override;


        /**
         * @brief Continuously runs the Stm32HalUartItDriver class.
         *
         * This method is called within the main program loop to continuously handle UART communication operations.
         * It inherits the `loop` method from the `AbstractDriver` class and performs any additional operations
         * specific to the `Stm32HalUartItDriver` class.
         */
        void loop() override;


        /**
         * @brief Transmit data over UART using interrupt-based transmission.
         *
         * This method transmits the given string over UART using interrupt-based transmission. It checks if the UART
         * is in the ready state before proceeding with the transmission. If the UART is not ready, it returns 0 to
         * indicate that the transmission was not successful. If the UART is ready, it calculates the size of the string
         * to transmit based on the minimum of the string length and the size of the transmit buffer. It then clears the
         * transmit buffer and copies the string to transmit into the buffer. Finally, it calls the HAL_UART_Transmit_IT
         * function to initiate the interrupt-based transmission. If the transmission is successful, it returns the size
         * of the transmitted data. Otherwise, it returns 0 to indicate that the transmission was not successful.
         *
         * @param str Pointer to the string to transmit.
         * @param strlen Length of the string to transmit.
         *
         * @return The size of the transmitted data if the transmission was successful, 0 otherwise.
         */
        size_t transmit(const uint8_t *str, size_t strlen) override {
            if (huart->gState != HAL_UART_STATE_READY) {
                return 0;
            }
            size_t sz = strlen > sizeof tx_buff ? sizeof tx_buff : strlen;
            memset(tx_buff, 0, sizeof tx_buff);
            memcpy(tx_buff, str, sz);
            if (HAL_OK == HAL_UART_Transmit_IT(huart, tx_buff, sz)) {
                return sz;
            }
            return 0;
        }

        /**
         * @brief Check the TX buffer and initiate sending.
         *
         * This method checks the TX buffer for data and initiates the sending process by calling the `_txIsr` method.
         * It is called internally and should not be called directly.
         */
        void checkTxBufferAndSend() override;

    private:
        /**
         * @brief Pointer to an instance of the UART_HandleTypeDef structure.
         *
         * This variable is used to store the handle of the UART peripheral. The UART_HandleTypeDef structure is a HAL
         * UART handle containing various configuration and state information for the UART.
         */
        UART_HandleTypeDef *huart;

        /**
         * @brief The buffer used for transmitting data via UART.
         *
         * This buffer is an array of uint8_t. It is used for storing the data to be transmitted via UART. The data can
         * be written to this buffer using appropriate functions/methods, and then sent out via UART using the relevant
         * UART driver functionality.
         */
        uint8_t tx_buff[LIBSMART_STM32SERIAL_HAL_UART_IT_BUFFER_SIZE_TX] = {};


        /**
         * @brief The buffer for receiving data in the Stm32HalUartItDriver class.
         *
         * This buffer is used to store the received data from the UART module.
         */
        uint8_t rx_buff[LIBSMART_STM32SERIAL_HAL_UART_IT_BUFFER_SIZE_RX] = {};
    };
}

#endif //LIBSMART_STM32SERIAL_STM32HALUARTITDRIVER_HPP
#endif
