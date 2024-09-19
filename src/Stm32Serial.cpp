/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Stm32Serial.hpp"
#include "AbstractDriver.hpp"


Stm32Serial::Stm32Serial::Stm32Serial(
    AbstractDriver *driver,
    Stm32Common::StreamSession::ManagerInterface *session_mgr,
    Stm32ItmLogger::LoggerInterface *logger)
    : Loggable(logger),
      StreamSessionAware(session_mgr),
      driver(driver) {
    driver->setSerialInstance(this);
    driver->setLogger(logger);
}


void Stm32Serial::Stm32Serial::begin(unsigned long baud, uint8_t config) {
    log()->setSeverity(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("Stm32Serial::Stm32Serial::begin()");

    // auto session = getSession();
    // LIBSMART_UNUSED(session);

    driver->begin(baud, config);
    isRunning = true;
}


void Stm32Serial::Stm32Serial::begin(unsigned long baud) {
    begin(baud, DEFAULT_CONFIG);
}


void Stm32Serial::Stm32Serial::begin() {
    begin(DEFAULT_BAUD, DEFAULT_CONFIG);
}


void Stm32Serial::Stm32Serial::setup() {
    begin();
}


void Stm32Serial::Stm32Serial::end() {
    flush();
    driver->end();
    isRunning = false;
}


Stm32Serial::Stm32Serial::operator bool() {
    return driver->isConnected();
}


void Stm32Serial::Stm32Serial::loop() {
    if (!isRunning) return;
    driver->loop();
    getSessionManager()->loop();
}

void Stm32Serial::Stm32Serial::dataReadyTx(Stm32Common::StreamSession::StreamSessionInterface *session) {
    if (sessionId == 0) {
        sessionId = session->getId();
    }
    driver->checkTxBufferAndSend();
}

size_t Stm32Serial::Stm32Serial::write(uint8_t data) {
    return getSession()->write(data);
}

void Stm32Serial::Stm32Serial::flush() {
    loop();
    getSessionManager()->flush();
    driver->flush();
}
