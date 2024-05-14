/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */



/**
 * Size of the rx buffer for serial interface.
 */
#define LIBSMART_STM32SERIAL_BUFFER_SIZE_RX 256



/**
 * Size of the tx buffer for serial interface.
 */
#define LIBSMART_STM32SERIAL_BUFFER_SIZE_TX 256


/**
 * Size of the rx buffer for serial interface.
 */
#define LIBSMART_STM32SERIAL_DRIVER_REGISTRY_SIZE 5


/**
 * Enable or disable the USB device CDC driver.
 */
#undef LIBSMART_STM32SERIAL_ENABLE_USB_CDC_DRIVER
//#define LIBSMART_STM32SERIAL_ENABLE_USB_CDC_DRIVER



/**
 * Enable or disable the HAL uart interrupt driver.
 */
#undef LIBSMART_STM32SERIAL_ENABLE_HAL_UART_IT_DRIVER
//#define LIBSMART_STM32SERIAL_ENABLE_HAL_UART_IT_DRIVER


/**
 * Size of the TX buffer for sending by interrupt.
 */
#define LIBSMART_STM32SERIAL_HAL_UART_IT_BUFFER_SIZE_TX 32


/**
 * Size of the RX buffer for reception by interrupt.
 */
#define LIBSMART_STM32SERIAL_HAL_UART_IT_BUFFER_SIZE_RX 32


/**
 * Enable or disable the HAL uart ThreadX poll driver.
 */
#undef LIBSMART_STM32SERIAL_ENABLE_HAL_UART_THREADX_POLL_DRIVER
//#define LIBSMART_STM32SERIAL_ENABLE_HAL_UART_THREADX_POLL_DRIVER
