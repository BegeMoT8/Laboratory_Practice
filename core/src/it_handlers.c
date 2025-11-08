#include <stdint.h>
#include <stdbool.h>
#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"
#include <it_handlers.h>
extern uint8_t LedState; // внешняя переменная состояния светодиода
volatile uint8_t btn_count = 0;

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