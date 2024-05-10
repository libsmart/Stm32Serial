/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SERIAL_STM32USBCDCDRIVER_HPP
#define LIBSMART_STM32SERIAL_STM32USBCDCDRIVER_HPP

#include "AbstractDriver.hpp"
#include "Stm32Serial.hpp"
#include "usbd_cdc_if.h"
#include "usbd_core.h"
#include "Helper.hpp"
#include <cstddef>

extern uint8_t UserTxBufferFS[];

namespace Stm32Serial {
    class Stm32UsbCdcDriver : public AbstractDriver {
        friend class Stm32Serial;

    public:
        Stm32UsbCdcDriver(USBD_HandleTypeDef *pdev, const char *name)
                : pdev(pdev),
                  AbstractDriver(name, (uint32_t) &pdev->id) {
            resetPin();
        };

        Stm32UsbCdcDriver(USBD_HandleTypeDef *pdev, uint32_t uniqueId)
                : pdev(pdev), AbstractDriver(uniqueId) {
            resetPin();
        };

        explicit Stm32UsbCdcDriver(USBD_HandleTypeDef *pdev)
                : pdev(pdev),
                  AbstractDriver((uint32_t) &pdev->id) {
            resetPin();
        };



        void begin(unsigned long baud, uint8_t config) override {
            AbstractDriver::begin(baud, config);
            self = this;
            USBD_Interface_fops_FS.Receive=Stm32UsbCdcDriver::CDC_Receive_FS;



            //tx_thread_sleep(UX_MS_TO_TICK(500));
            // Hardware reset rendered harmless!

        }


    protected:
        static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len) {
            return self->receive(Buf, Len);
        }

        int8_t receive(uint8_t* Buf, const uint32_t *Len) {
            auto rxBuffer = getRxBuffer();
            auto txBuffer = getTxBuffer();
            rxBuffer->write(Buf, *Len);
            txBuffer->write(Buf, *Len);
            memset(Buf, 0, APP_RX_DATA_SIZE);
            USBD_CDC_SetRxBuffer(pdev, Buf);
            USBD_CDC_ReceivePacket(pdev);
            return (USBD_OK);
        }

        size_t transmit(const uint8_t *str, size_t strlen) override {
            auto txBuffer = getTxBuffer();

            // Check, if interface is busy
            auto *hcdc = (USBD_CDC_HandleTypeDef *) pdev->pClassData;
            if (hcdc->TxState != 0) {
                return 0;
            }

            size_t sz = std::min(strlen, (size_t) APP_TX_DATA_SIZE);
            memset(UserTxBufferFS, 0, APP_TX_DATA_SIZE);
            memcpy(UserTxBufferFS, str, sz);
            auto ret = CDC_Transmit_FS(UserTxBufferFS, sz);
            if (ret == USBD_OK) {
                return sz;
            }
            return 0;
        }

        void checkTxBufferAndSend() override {
            auto txBuffer = getTxBuffer();
            if (txBuffer->getLength() > 0) {
                auto sentBytes = transmit(txBuffer->getReadPointer(), txBuffer->getLength());
                txBuffer->remove(sentBytes);
            }
        }

        void resetPin() {
            // Rendering hardware reset harmless (no need to replug USB cable)
            GPIO_InitTypeDef GPIO_InitStruct = {0};

            // GPIO Ports Clock Enable
            __HAL_RCC_GPIOA_CLK_ENABLE();

            // Configure GPIO pin Output Level
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);

            // Configure GPIO pin : PA12, a.k.a. USB_DP
            GPIO_InitStruct.Pin = GPIO_PIN_12;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

            // Sleep for 100 ms
            //delay(100);
        }


    private:
        USBD_HandleTypeDef *pdev;
        static Stm32UsbCdcDriver *self;
    };
}

#endif //LIBSMART_STM32SERIAL_STM32USBCDCDRIVER_HPP
