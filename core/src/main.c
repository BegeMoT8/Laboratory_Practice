// вариант №3
#include <init.h>
#include <it_handlers.h>

// Полупериоды для частот мерцания светодиодов в мс
#define Frequency1 1250 // Частота 0.4 Гц
#define Frequency2 455  // Частота 1.1 Гц
#define Frequency3 263  // Частота 1.9 Гц

extern volatile uint8_t btn_count;   // Счетчик коротких нажатий
extern volatile uint8_t btn_hold_2s; // Счетчик удержаний 2 сек
extern volatile uint8_t btn_hold_4s; // Счетчик удержаний 4 сек
extern volatile uint32_t GlobalTickCount;

int main(void)
{
    RCC_Init();     // Инициализация системы тактирования RCC
    GPIO_Init();    // Инициализация поротв GPIO
    ITR_Init();     // Инициализация контроллера прерываний
    SysTick_Init(); // Инициализация системного таймера

    uint32_t last_toggle_time = 0;
    bool led_state = true; // true = включены, false = выключены
    uint32_t current_period = Frequency1;

    while (1)
    {
        uint32_t current_time = GlobalTickCount;

        // Функция 3: Определяем режим работы
        bool flicker_mode = (btn_hold_4s % 2 == 1);

        if (!flicker_mode)
        {
            // Режим простого свечения
            led_on(btn_count);
            last_toggle_time = current_time; // Сброс таймера
        }
        else
        {
            // Функция 2: Изменение частоты мерцания
            switch (btn_hold_2s % 3)
            {
            case 0: // Частота 0.4 Гц
                current_period = Frequency1;
                break;
            case 1: // Частота 1.1 Гц
                current_period = Frequency2;
                break;
            case 2: // Частота 1.9 Гц
                current_period = Frequency3;
                break;
            }
            // Неблокирующее мерцание
            if ((uint32_t)(current_time - last_toggle_time) >= current_period)
            {
                last_toggle_time = current_time;
                led_state = !led_state;

                (led_state) ? led_on(btn_count) : led_off();
            }
        }
    }
}
