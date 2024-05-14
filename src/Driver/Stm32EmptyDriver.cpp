/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "Stm32EmptyDriver.hpp"

using namespace Stm32Serial;


size_t Stm32EmptyDriver::transmit(const uint8_t *str, size_t strlen) {
    return strlen;
}
