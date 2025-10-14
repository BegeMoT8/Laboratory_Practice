#include "init.h"

void GPIO_Ini()
{
    // // включаем тактирование на пины GPIOCEN GPIOBEN GPIOAEN

    // включаем тактирование на пины GPIOB (адрес RCC + адрес RCC_AHB1ENR) |= бит GPIOB
    RCC_GPIO_EN |= RCC_GPIOB_EN;                   // собственный макрос
    (*(uint32_t *)(0x40023800UL + 0x30UL)) |= 0x2; // прямое обращение к памяти
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);    // библиотека CMIS

    // //настройка кнопок
    // конфигурируем все пины на вход
    CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE10 | GPIO_MODER_MODE11);
    // тип выхода push pull
    CLEAR_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10);
    // скорость работы - средняя
    SET_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR8_0 | GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR10_0 | GPIO_OSPEEDER_OSPEEDR11_0);
    // подтяжка резистора pull up
    SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0);

    // Настройка макросом (желтый 0)
    GPIOB_MODER |= GPIOB_MODE_PIN0_OUT;
    // GPIOB_OTYPER |= GPIOB_OTYPER_PIN0_PP;

    // настройка прямым обращением к памяти (красный светодиод 14)
    //(адрес GPIOB + адрес MODER) для настройки режима порта (00 input; 01 output)
    (*(uint32_t *)(0x40020400UL + 0x00UL)) |= 0x10000000;
    // скорость работы порта - средняя
    (*(uint32_t *)(0x40020400UL + 0x08UL)) |= 0x10000000;

    // настройка CMIS (синий светодиод 7)
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODE7);
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE7_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT7_Msk);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR7_0);
}

// OPTIMIZE: возможно стоит добавить верхнюю границу для обработки зажатой кнопки
bool button_bounce_read(uint32_t idr_mask)
{
    bool press = false;
    int32_t bounce = 0;
    do
    {
        if (READ_BIT(GPIOC->IDR, idr_mask) == 0)
        {
            bounce++;
            press = true;
            continue;
        }
        else
        {
            bounce--;
        }
    } while (bounce > 0);
    return press;
}


// NOTE: принимает аргументом номера битов регистра MODER для установки режима выхода
//  остальные порты кнопок переходят в режим входа, и биты регистра BSRR для подачи высокого сигнала на кнопки в режиме выход
void changeConf(uint32_t PORT0_bit, uint32_t PORT1_bit, uint32_t PORT0_LIGTH, uint32_t PORT1_LIGTH, bool noRepeat)
{
    if (noRepeat == true)
    {
        CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE10);
        SET_BIT(GPIOC->MODER, PORT0_bit | PORT1_bit);
        SET_BIT(GPIOC->BSRR, PORT0_LIGTH | PORT1_LIGTH);
    }
}

// NOTE: принимает аргументами:тип порта на котором находится кнопка (GPIOx), бит для чтения с порта GPIOx,
// бит для записи в GPIOB, бит для очистки бита в GPIOB
void button_light_led(GPIO_TypeDef *GPIOx, uint32_t IDR_bit, uint32_t BSRR_BS, uint32_t BSRR_BR)
{
    if (READ_BIT(GPIOx->IDR, IDR_bit) == 0)
    {
        SET_BIT(GPIOB->BSRR, BSRR_BS);
    }
    else
    {
        SET_BIT(GPIOB->BSRR, BSRR_BR);
    }
}

// NOTE: для доп задания
//  принимает число для подстановки в отношение (n/20) для задания n итераций в течение которых светодиод включен
//  бит для включения и выключения высокого сигнала на порте
void mode_light(uint8_t ratio, uint32_t BSRR_BSx, uint32_t BSRR_BRx)
{
    for (int i = 0; i < ratio; i++)
    {
        SET_BIT(GPIOB->BSRR, BSRR_BSx);
    }
    for (int i = 0; i < 20; i++)
    {
        SET_BIT(GPIOB->BSRR, BSRR_BRx);
    }
}

// NOTE: дополнительное задание
void changing_brightness()
{
    static uint8_t bright = 1, counter_press8 = 0, counter_press9 = 0, counter_press10 = 0;
    if (button_bounce_read(GPIO_IDR_IDR_11) == true)
    {
        bright++;
    }
    if (bright > 3)
    {
        bright = 1;
    }

    if (button_bounce_read(GPIO_IDR_IDR_8) == true)
    {
        counter_press8++;
    }
    if (button_bounce_read(GPIO_IDR_IDR_9) == true)
    {
        counter_press9++;
    }
    if (button_bounce_read(GPIO_IDR_IDR_10) == true)
    {
        counter_press10++;
    }

    if (counter_press8 % 2 == 1)
    {
        if (bright == 1)
        {
            mode_light(1, GPIO_BSRR_BS7_Msk, GPIO_BSRR_BR7_Msk);
        }
        else if (bright == 2)
        {
            mode_light(12, GPIO_BSRR_BS7_Msk, GPIO_BSRR_BR7_Msk);
        }
        else
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7_Msk);
        }
    }
    else
    {
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7_Msk);
    }
    if (counter_press9 % 2 == 1)
    {
        if (bright == 1)
        {
            mode_light(1, GPIO_BSRR_BS14_Msk, GPIO_BSRR_BR14_Msk);
        }
        else if (bright == 2)
        {
            mode_light(12, GPIO_BSRR_BS14_Msk, GPIO_BSRR_BR14_Msk);
        }
        else
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS14_Msk);
        }
    }
    else
    {
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14_Msk);
    }
    if (counter_press10 % 2 == 1)
    {
        if (bright == 1)
        {
            mode_light(6, GPIO_BSRR_BS0_Msk, GPIO_BSRR_BR0_Msk);
        }
        else if (bright == 2)
        {
            mode_light(15, GPIO_BSRR_BS0_Msk, GPIO_BSRR_BR0_Msk);
        }
        else
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0_Msk);
        }
    }
    else
    {
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0_Msk);
    }
}

// NOTE: основное задание
void main_task()
{

    static int8_t counter_press = 0, old_counter_press = 0;
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
        button_light_led(GPIOC, GPIO_IDR_IDR_10, GPIO_BSRR_BS7_Msk, GPIO_BSRR_BR7_Msk);
    }
    else if (counter_press == 2)
    {
        changeConf(GPIO_MODER_MODER8_0, GPIO_MODER_MODER10_0, GPIO_BSRR_BS8, GPIO_BSRR_BS10, (old_counter_press != counter_press));
        old_counter_press = counter_press;
        // 9 на вход остальные на выход
        button_light_led(GPIOC, GPIO_IDR_IDR_9, GPIO_BSRR_BS14_Msk, GPIO_BSRR_BR14_Msk);
    }
    else if (counter_press == 3)
    {
        changeConf(GPIO_MODER_MODER9_0, GPIO_MODER_MODER10_0, GPIO_BSRR_BS9, GPIO_BSRR_BS10, (old_counter_press != counter_press));
        old_counter_press = counter_press;
        // 8 на вход остальные на выход
        button_light_led(GPIOC, GPIO_IDR_IDR_8, GPIO_BSRR_BS0_Msk, GPIO_BSRR_BR0_Msk);
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
        button_light_led(GPIOC, GPIO_IDR_IDR_10, GPIO_BSRR_BS7_Msk, GPIO_BSRR_BR7_Msk);
        button_light_led(GPIOC, GPIO_IDR_IDR_9, GPIO_BSRR_BS14_Msk, GPIO_BSRR_BR14_Msk);
        button_light_led(GPIOC, GPIO_IDR_IDR_8, GPIO_BSRR_BS0_Msk, GPIO_BSRR_BR0_Msk);
    }
    else if (counter_press > 5)
    {
        counter_press = 1;
    }
}
