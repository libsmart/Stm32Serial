/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../../../src/libsmart_config.dist.hpp"
#include "../Lib/Stm32Common/src/libsmart_config.dist.hpp"

/**
 * Enable or disable the USB device CDC driver.
 */
#undef LIBSMART_STM32SERIAL_ENABLE_USB_CDC_DRIVER
#define LIBSMART_STM32SERIAL_ENABLE_USB_CDC_DRIVER
