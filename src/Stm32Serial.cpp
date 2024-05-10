/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Stm32Serial.hpp"
#include "AbstractDriver.hpp"
//#include "Stm32GcodeRunner.hpp"

Stm32Serial::Stm32Serial::Stm32Serial(AbstractDriver *driver) : driver(driver) {
    driver->setSerialInstance(this);
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


size_t Stm32Serial::Stm32Serial::getWriteBuffer(uint8_t *&buffer) {
    buffer = txBuffer.getWritePointer();
    return txBuffer.getRemainingSpace();
}


size_t Stm32Serial::Stm32Serial::setWrittenBytes(size_t size) {
    return txBuffer.add(size);
}

/*
void Stm32Serial::Stm32Serial::loop() {
    Stm32Common::buf_size_signed_t posR = rxBuffer.findPos('\r');
    Stm32Common::buf_size_signed_t posN = rxBuffer.findPos('\n');
    const Stm32Common::buf_size_signed_t eolPos = std::max(posR, posN);
    if (eolPos > 0) {
        Stm32GcodeRunner::AbstractCommand *cmd{};
        const size_t cmdLen = std::min(posR < 0 ? SIZE_MAX : posR, posN < 0 ? SIZE_MAX : posN);
        auto parserRet = Stm32GcodeRunner::parser->parseString(
            cmd, reinterpret_cast<const char *>(rxBuffer.getReadPointer()), cmdLen);
        rxBuffer.remove(eolPos + 1);

        if (parserRet == Stm32GcodeRunner::Parser::parserReturn::OK) {
            Debugger_log(DBG, "Found command: %s", cmd->getName());
            Stm32GcodeRunner::CommandContext *cmdCtx{};
            Stm32GcodeRunner::worker->createCommandContext(cmdCtx);
            if (cmdCtx == nullptr) {
                txBuffer.println("ERROR: Command buffer full");
                return;
            }
            cmdCtx->setCommand(cmd);

            cmdCtx->registerOnWriteFunction([cmdCtx, this]() {
                // Debugger_log(DBG, "onWriteFn()");
                if (cmdCtx->outputLength() > 0) {
                    const auto result = cmdCtx->outputRead(
                        reinterpret_cast<char *>(this->txBuffer.getWritePointer()),
                        this->txBuffer.getRemainingSpace());
                    this->txBuffer.setWrittenBytes(result);
                }
            });

            cmdCtx->registerOnCmdEndFunction([cmdCtx]() {
                // Debugger_log(DBG, "onCmdEndFn()");
                Stm32GcodeRunner::worker->deleteCommandContext(cmdCtx);
            });

            Stm32GcodeRunner::worker->enqueueCommandContext(cmdCtx);
        } else if (parserRet == Stm32GcodeRunner::Parser::parserReturn::UNKNOWN_COMMAND) {
            txBuffer.println("ERROR: UNKNOWN COMMAND");
        } else if (parserRet == Stm32GcodeRunner::Parser::parserReturn::GARBAGE_STRING) {
            txBuffer.println("ERROR: UNKNOWN COMMAND");
        } else {
            // txBuffer.println("ERROR: ONLY WHITESPACE");
        }
    }


    driver->checkTxBufferAndSend();
}
 */

void Stm32Serial::Stm32Serial::loop() {
    driver->checkTxBufferAndSend();
}

void Stm32Serial::Stm32Serial::txBufferClass::onWrite() {
    StringBuffer::onWrite();
    self.driver->checkTxBufferAndSend();
}

