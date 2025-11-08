// вариант №3
#include <init.h>
#include <it_handlers.h>

#define FLICKER_PERIOD 2000 // Период мерцания светодиода
uint16_t GlobalTickCount;
uint8_t LedState;
int main(void)
{
    GPIO_Init();     // Инициализация поротв GPIO
    RCC_Init();      // Инициализация системы тактирования RCC
    ITR_Init();      // Инициализация контроллера прерываний
    SysTick_Init(); // Инициализация системного таймера
    while (1)
    {
        if (LedState) // Если нажали один раз на кнопку светодиод включится
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
        else // Если нажали ещё раз светодиод выключится
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7);
        if (GlobalTickCount >= FLICKER_PERIOD / 2) // Если прошло 1000 прерываний системного таймера = 1 секунда
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);   // Включаем светодиод
            if (GlobalTickCount >= FLICKER_PERIOD) // Если прошло 2000 прерываний системного таймера = 2 секунды
            {
                GlobalTickCount = 0; // Обнуляем переменную счётчика системного таймера
            }
        }
        else
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7); // Иначе выключаем светодиод
        }
    }
}
