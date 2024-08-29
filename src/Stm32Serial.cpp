/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Stm32Serial.hpp"
#include "AbstractDriver.hpp"


Stm32Serial::Stm32Serial::Stm32Serial(AbstractDriver *driver, Stm32ItmLogger::LoggerInterface *logger)
    : Loggable(logger), driver(driver) {
    driver->setSerialInstance(this);
    driver->setLogger(logger);
}


void Stm32Serial::Stm32Serial::begin(unsigned long baud, uint8_t config) {
    driver->begin(baud, config);
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

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
size_t Stm32Serial::Stm32Serial::getWriteBuffer(uint8_t *&buffer) {
    buffer = txBuffer.getWritePointer();
    return txBuffer.getRemainingSpace();
}


size_t Stm32Serial::Stm32Serial::setWrittenBytes(size_t size) {
    return txBuffer.add(size);
}
#endif


void Stm32Serial::Stm32Serial::loop() {
    driver->loop();
    driver->checkTxBufferAndSend();
}


void Stm32Serial::Stm32Serial::txBufferClass::onWrite() {
    StringBuffer::onWrite();
    self.driver->checkTxBufferAndSend();
}

