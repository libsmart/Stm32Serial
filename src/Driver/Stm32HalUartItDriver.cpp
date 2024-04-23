/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Stm32HalUartItDriver.hpp"
#include "globals.hpp"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) {
        Debugger_log(DBG, "HAL_UART_RxCpltCallback()");
        Serial3Driver.isr(16);
    }
//        HAL_UART_Receive_IT(&huart3, rx_buff, 10); //You need to toggle a breakpoint on this line!
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART3) {
//        Debugger_log(DBG, "HAL_UARTEx_RxEventCallback()");
        Serial3Driver.isr(Size);
    }
}



void Stm32HalUartItDriver_isr(UART_HandleTypeDef *huart) {
//    Debugger_log(DBG, "Stm32HalUartItDriver_isr() RxState=0x%2x", huart->RxState);
//    if (huart->RxState == HAL_UART_STATE_READY) {
//        Serial3Driver.isr();
//    }
}

