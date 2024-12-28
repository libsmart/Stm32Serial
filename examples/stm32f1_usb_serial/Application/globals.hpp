/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

/**
 * This file holds exports for the global variables, defined in globals.cpp.
 * @see globals.cpp
 */

#ifndef EASY_SMART_STM32_skeleton_GLOBALS_HPP
#define EASY_SMART_STM32_skeleton_GLOBALS_HPP

#include "main.h"
#include "globals.h"
#include "Stm32Serial.hpp"
#include "Driver/Stm32UsbCdcDriver.hpp"
#include <cstdint>
#include "usb_device.h"
#include "StreamSession/EchoStreamSession.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    inline uint32_t dummyCpp;

#ifdef __cplusplus
}
#endif

inline Stm32ItmLogger::Stm32ItmLogger &Logger = Stm32ItmLogger::logger;

extern USBD_HandleTypeDef hUsbDeviceFS;
inline Stm32Common::StreamSession::Manager<Stm32Common::StreamSession::EchoStreamSession, 1> echoStreamSessionManager(&Logger);
inline Stm32Serial::Stm32UsbCdcDriver UsbSerialDriver(&hUsbDeviceFS, "UsbSerialDriver");
inline Stm32Serial::Stm32Serial Serial(&UsbSerialDriver, &echoStreamSessionManager, &Logger);


#endif
