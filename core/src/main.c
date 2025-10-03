// вариант №3
#include <init.h>



int main(void)
{
    GPIO_Ini();
    int8_t counter_press = 0, old_counter_press = 0;
    while (1)
    {

        if (button_bounce_read(GPIO_IDR_IDR_11) == true)
        {
            counter_press++;
        }

        // NOTE: функция смены конфигурации вызывается только один раз

        if (counter_press == 1)
        {
            changeConf(GPIO_MODER_MODER8_0, GPIO_MODER_MODER9_0, GPIO_BSRR_BS8, GPIO_BSRR_BS9, (old_counter_press != counter_press));
            old_counter_press = counter_press;
            // 10 на вход, остальные на выход
            button_light_led(GPIO_IDR_IDR_10, GPIO_BSRR_BS7_Msk, GPIO_BSRR_BR7_Msk);
        }
        else if (counter_press == 2)
        {
            changeConf(GPIO_MODER_MODER8_0, GPIO_MODER_MODER10_0, GPIO_BSRR_BS8, GPIO_BSRR_BS10, (old_counter_press != counter_press));
            old_counter_press = counter_press;
            // 9 на вход остальные на выход
            button_light_led(GPIO_IDR_IDR_9, GPIO_BSRR_BS14_Msk, GPIO_BSRR_BR14_Msk);
        }
        else if (counter_press == 3)
        {
            changeConf(GPIO_MODER_MODER9_0, GPIO_MODER_MODER10_0, GPIO_BSRR_BS9, GPIO_BSRR_BS10, (old_counter_press != counter_press));
            old_counter_press = counter_press;
            // 8 на вход остальные на выход
            button_light_led(GPIO_IDR_IDR_8, GPIO_BSRR_BS0_Msk, GPIO_BSRR_BR0_Msk);
        }
        else if (counter_press == 4)
        {
            // все порты в режим выхода
            if (old_counter_press != counter_press)
            {
                SET_BIT(GPIOC->MODER, GPIO_MODER_MODER8_0);
                old_counter_press = counter_press;
            }
        }
        else if ((counter_press == 5) | (counter_press == 0))
        {
            // все порты в режим входа
            if (old_counter_press != counter_press)
            {
                CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE10);
                old_counter_press = counter_press;
            }
            button_light_led(GPIO_IDR_IDR_10, GPIO_BSRR_BS7_Msk, GPIO_BSRR_BR7_Msk);
            button_light_led(GPIO_IDR_IDR_9, GPIO_BSRR_BS14_Msk, GPIO_BSRR_BR14_Msk);
            button_light_led(GPIO_IDR_IDR_8, GPIO_BSRR_BS0_Msk, GPIO_BSRR_BR0_Msk);
        }
        else if (counter_press > 5)
        {
            counter_press = 1;
        }
    }
}