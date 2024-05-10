/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <libsmart_config.hpp>
#ifdef LIBSMART_STM32SERIAL_ENABLE_USB_CDC_DRIVER

#include "Stm32UsbCdcDriver.hpp"

using namespace Stm32Serial;

Stm32UsbCdcDriver *Stm32UsbCdcDriver::self = {};

#endif