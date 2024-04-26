/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Stm32Serial.hpp"
#include "AbstractDriver.hpp"

Stm32Serial::Stm32Serial::Stm32Serial(AbstractDriver *driver) : driver(driver) {
    driver->setSerialInstance(this);
}


void Stm32Serial::Stm32Serial::begin(unsigned long baud, uint8_t config) {
    driver->begin(baud, config);
//            write("Hallo Welt\r\n");

    print("Hallo Welt ");
    print(17L);
    print(' ');
    print(3.1415);
    println();
}


void Stm32Serial::Stm32Serial::begin(unsigned long baud) {
    begin(baud, DEFAULT_CONFIG);
}


void Stm32Serial::Stm32Serial::begin() {
    begin(DEFAULT_BAUD, DEFAULT_CONFIG);
}


void Stm32Serial::Stm32Serial::end() {
    flush();
    driver->end();
}


int Stm32Serial::Stm32Serial::available() {
    return rxBuffer.getLength() > INT_MAX ? INT_MAX : (int) rxBuffer.getLength();
}


int Stm32Serial::Stm32Serial::peek() {
    return rxBuffer.peek();
}


int Stm32Serial::Stm32Serial::read() {
    return rxBuffer.read();
}


int Stm32Serial::Stm32Serial::availableForWrite() {
    return txBuffer.getRemainingSpace() > INT_MAX ? INT_MAX : (int) txBuffer.getRemainingSpace();
}


void Stm32Serial::Stm32Serial::flush() {
    driver->flush();
}


size_t Stm32Serial::Stm32Serial::write(uint8_t uint8) {
    if (txBuffer.write(uint8) == 1) {
        driver->checkTxBufferAndSend();
        return 1;
    } else return 0;
}


Stm32Serial::Stm32Serial::operator bool() {
    return driver->isConnected();
}


size_t Stm32Serial::Stm32Serial::getWriteBuffer(uint8_t *&buffer) {
    buffer = txBuffer.getWritePointer();
    return txBuffer.getRemainingSpace();
}


size_t Stm32Serial::Stm32Serial::setWrittenBytes(size_t size) {
    return txBuffer.add(size);
}


void Stm32Serial::Stm32Serial::loop() {

    if (rxBuffer.getLength() > 0) {
        Debugger_log(DBG, "Received: '%.*s'", rxBuffer.getLength(), rxBuffer.getReadPointer());
        rxBuffer.clear();
    }

    driver->checkTxBufferAndSend();
}


