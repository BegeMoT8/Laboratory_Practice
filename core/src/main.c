// вариант №3
#include <init.h>
#include <it_handlers.h>

#define FLICKER_PERIOD 2000     // Период мерцания светодиода
#define FLICKER_SEMIPERIOD 1000 // Полупериод мерцания светодиода

uint16_t GlobalTickCount;
uint8_t LedState;
int main(void)
{
    GPIO_Init();    // Инициализация поротв GPIO
    RCC_Init();     // Инициализация системы тактирования RCC
    ITR_Init();     // Инициализация контроллера прерываний
    SysTick_Init(); // Инициализация системного таймера
    while (1)
    {
        if (LedState)
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7); // Если нажали один раз на кнопкусветодиод включится
        }
        else
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7); // Если нажали ещё раз светодиод выключится
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7); // Включаем светодиод
        }

        User_Delay(FLICKER_SEMIPERIOD);      // Ожидаем 1 секунду
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7); // Выключаем светодиод
        User_Delay(FLICKER_SEMIPERIOD);      // Ожидаем 1 секунду
    }
}
