/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

/**
 * This file holds the main setup() and loop() functions for C++ code.
 * If a RTOS is used, loop() is called in the main task and setup() is called before RTOS initialization.
 * @see App_ThreadX_Init() in Core/Src/app_threadx.c
 */

#include "main.hpp"
#include "globals.hpp"


/**
 * @brief Setup function.
 * This function is called once at the beginning of the program before ThreadX is initialized.
 * @see main() in Core/Src/main.c
 */
void setup() {
    Stm32ItmLogger::logger.setSeverity(Stm32ItmLogger::LoggerInterface::Severity::INFORMATIONAL)
            ->println("::setup()");

    dummyCpp = 0;
    dummyCandCpp = 0;
    Serial.begin();
    Serial.print('\0');
}


/**
 * @brief This function is the main loop that executes continuously.
 * The function is called inside the mainLoopThread().
 * @see mainLoopThread() in AZURE_RTOS/App/app_azure_rtos.c
 */
void loop() {
    dummyCpp++;
    dummyCandCpp++;

    Serial.loop();
    Serial.print("counter = ");
    Serial.print(dummyCpp);
    Serial.println();
    Serial.flush();


    HAL_GPIO_WritePin(LED1_GRN_GPIO_Port, LED1_GRN_Pin, dummyCpp & 2 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_ORG_GPIO_Port, LED2_ORG_Pin, dummyCpp & 4 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED3_RED_GPIO_Port, LED3_RED_Pin, dummyCpp & 8 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED4_BLU_GPIO_Port, LED4_BLU_Pin, dummyCpp & 1 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_Delay(500);
}


/**
 * @brief This function handles fatal errors.
 * @see Error_Handler() in Core/Src/main.c
 */
void errorHandler() {
    HAL_GPIO_WritePin(LED1_GRN_GPIO_Port, LED1_GRN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED2_ORG_GPIO_Port, LED2_ORG_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED3_RED_GPIO_Port, LED3_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED4_BLU_GPIO_Port, LED4_BLU_Pin, GPIO_PIN_RESET);

    while (true) {
        for (uint32_t i = (SystemCoreClock / 10); i > 0; i--) { UNUSED(i); }
        HAL_GPIO_TogglePin(LED1_GRN_GPIO_Port, LED1_GRN_Pin);
        HAL_GPIO_TogglePin(LED2_ORG_GPIO_Port, LED2_ORG_Pin);
        HAL_GPIO_TogglePin(LED3_RED_GPIO_Port, LED3_RED_Pin);
        HAL_GPIO_TogglePin(LED4_BLU_GPIO_Port, LED4_BLU_Pin);
    }
}
