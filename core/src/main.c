// вариант №3
#include <init.h>
#include <it_handlers.h>

extern volatile uint8_t btn_count;   // Счетчик коротких нажатий
extern volatile uint8_t btn_hold_2s; // Счетчик удержаний 2 сек
extern volatile uint8_t btn_hold_4s; // Счетчик удержаний 4 сек

int main(void)
{
    RCC_Init();     // Инициализация системы тактирования RCC
    GPIO_Init();    // Инициализация поротв GPIO
    ITR_Init();     // Инициализация контроллера прерываний
    SysTick_Init(); // Инициализация системного таймера

    // Создание структур Led для каждого из 6-ти светодиодов
    Led led1, led2, led3, led4, led5, led6;

    // Инициализация светодиодов
    Led_init(&led1, 1);
    Led_init(&led2, 2);
    Led_init(&led3, 3);
    Led_init(&led4, 4);
    Led_init(&led5, 5);
    Led_init(&led6, 6);

    while (1)
    {
        // Функция 3: Определяем режим работы
        bool flicker_mode = (btn_hold_4s % 2 == 1);

        if (!flicker_mode)
        {
            // Режим простого свечения - светодиоды горят постоянно
            if (btn_count == 0)
            {
                Led_off(&led1); Led_off(&led2); Led_off(&led3); Led_off(&led4); Led_off(&led5); Led_off(&led6);
            }
            else if (btn_count == 1)
            {
                Led_on(&led1); Led_off(&led2); Led_off(&led3); Led_off(&led4); Led_off(&led5); Led_off(&led6);
            }
            else if (btn_count == 2)
            {
                Led_on(&led1); Led_on(&led2); Led_off(&led3); Led_off(&led4); Led_off(&led5); Led_off(&led6);
            }
            else if (btn_count == 3)
            {
                Led_on(&led1); Led_on(&led2); Led_on(&led3); Led_off(&led4); Led_off(&led5); Led_off(&led6);
            }
            else if (btn_count == 4)
            {
                Led_on(&led1); Led_on(&led2); Led_on(&led3); Led_on(&led4); Led_off(&led5); Led_off(&led6);
            }
            else if (btn_count == 5)
            {
                Led_on(&led1); Led_on(&led2); Led_on(&led3); Led_on(&led4); Led_on(&led5); Led_off(&led6);
            }
            else if (btn_count == 6)
            {
                Led_on(&led1); Led_on(&led2); Led_on(&led3); Led_on(&led4); Led_on(&led5); Led_on(&led6);
            }
        }
        else
        {
            // Режим мерцания - каждый светодиод мерцает со своей частотой
            if (btn_count == 0)
            {
                Led_off(&led1); Led_off(&led2); Led_off(&led3); Led_off(&led4); Led_off(&led5); Led_off(&led6);
            }
            else if (btn_count == 1)
            {
                Led_flicker(&led1);
            }
            else if (btn_count == 2)
            {
                Led_flicker(&led1);
                Led_flicker(&led2);

            }
            else if (btn_count == 3)
            {
                Led_flicker(&led1);
                Led_flicker(&led2);
                Led_flicker(&led3);
            }
            else if (btn_count == 4)
            {
                Led_flicker(&led1);
                Led_flicker(&led2);
                Led_flicker(&led3);
                Led_flicker(&led4); 
            }
            else if (btn_count == 5)
            {
                Led_flicker(&led1);
                Led_flicker(&led2);
                Led_flicker(&led3);
                Led_flicker(&led4); 
                Led_flicker(&led5);
            }
            else if (btn_count == 6)
            {
                Led_flicker(&led1);
                Led_flicker(&led2);
                Led_flicker(&led3);
                Led_flicker(&led4); 
                Led_flicker(&led5);
                Led_flicker(&led6);
            }
        }
    }
}
