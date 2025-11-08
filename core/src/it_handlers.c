#include <stdint.h>
#include <stdbool.h>
#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"
#include <it_handlers.h>
extern uint8_t LedState; // внешняя переменная состояния светодиода
extern uint16_t GlobalTickCount;
volatile uint8_t btn_count = 0;

/**
 * @brief Обработчик прерывания по EXTI линии 13 (кнопка на PC13)
 * @details При срабатывании прерывания увеличивает счетчик нажатий кнопки на 2.
 * Если счетчик достигает 2, переключает состояние светодиода и сбрасывает счетчик.
 */
void EXTI15_10_IRQHandler(void)
{
    btn_count++;
    if (btn_count >= 2)
    {
        LedState = !LedState;
        btn_count = 0;
    }
    SET_BIT(EXTI->PR, EXTI_PR_PR13); // устанавливаем бит для остановки прерывания
}

/**
 * @brief Обработчик прерывания системного таймера SysTick
 * @details Увеличивает глобальный счетчик тиков на 1 при каждом срабатывании прерывания.
 */
void SysTick_Handler(void)
{
    GlobalTickCount++;
}