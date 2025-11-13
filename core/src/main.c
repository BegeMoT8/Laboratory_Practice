// вариант №3
#include <init.h>
#include <it_handlers.h>

extern volatile uint8_t btn_count;   // Счетчик коротких нажатий
extern volatile uint8_t btn_hold_4s; // Счетчик удержаний 4 сек
#define Led_Count 6

// Создание структур Led для каждого из 6-ти светодиодов
Led led1, led2, led3, led4, led5, led6;

/**
 * @brief главная функция программы
 * @details иницилизация систем и основной цикл с двумя режимами работы
 * @return int
 */
int main(void)
{
    RCC_Init();     // Инициализация системы тактирования RCC
    GPIO_Init();    // Инициализация портов GPIO
    ITR_Init();     // Инициализация контроллера прерываний
    SysTick_Init(); // Инициализация системного таймера

    Led *led_adress[] = {&led1, &led2, &led3, &led4, &led5, &led6};

    // Инициализация светодиодов
    for (uint8_t i = 1; i <= Led_Count; i++)
    {
        Led_init(led_adress[i - 1], i);
    }

    while (1)
    {
        // Определяем режим работы
        bool flicker_mode = (btn_hold_4s % 2 == 1);

        if (flicker_mode)
        {
            // режим постоянного свечения
            for (int i = 0; i < Led_Count; i++)
            {
                if (i < btn_count)
                {
                    Led_on(led_adress[i]);
                }
                else
                {
                    Led_off(led_adress[i]);
                }
            }
        }
        else
        {
            // режим мерцания
            if (btn_count == 0) // выключение всех led
            {
                for (uint8_t i = 1; i <= Led_Count; i++)
                {
                    Led_off(led_adress[i - 1]);
                }
            }
            else // мерцание светодиодов
            {
                for (uint8_t i = 1; i <= btn_count; i++)
                {
                    Led_flicker(led_adress[i - 1]);
                }
            }
        }
    }
}
