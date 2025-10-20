#include "init.h"

volatile bool btn1_st = false, btn2_st = false, btn3_st = false, btn4_st = false;
volatile bool red_led = false, blue_led = false, yellow_led = false;

void GPIO_Ini()
{
    // NOTE: используемые порты: PC11; PD2; PG2; PE2;

    // включаем тактирование на пины GPIOC/D/E/G/B
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOBEN);

    // PC11 - настройка через ПРЯМОЕ ОБРАЩЕНИЕ К ПАМЯТИ

    // Базовый адрес GPIOC: 0x40020800
    // MODER offset: 0x00, OTYPER: 0x04, OSPEEDR: 0x08, PUPDR: 0x0C

    // Конфигурируем на вход (MODER = 00 для пина 11, биты 22-23)
    (*(uint32_t *)(0x40020800UL + 0x00UL)) &= ~(0x3UL << 22);

    // Настройка типа выхода push pull (OTYPER бит 11 = 0)
    // NOTE: для входа это не критично, но для единообразия сбросим
    (*(uint32_t *)(0x40020800UL + 0x04UL)) &= ~(0x1UL << 11);

    // Настройка скорости работы - средняя (OSPEEDR = 01 для пина 11, биты 22-23)
    (*(uint32_t *)(0x40020800UL + 0x08UL)) |= (0x1UL << 22);
    (*(uint32_t *)(0x40020800UL + 0x08UL)) &= ~(0x2UL << 22);

    // Подтяжка резистора - pull up (PUPDR = 01 для пина 11, биты 22-23)
    (*(uint32_t *)(0x40020800UL + 0x0CUL)) |= (0x1UL << 22);
    (*(uint32_t *)(0x40020800UL + 0x0CUL)) &= ~(0x2UL << 22);

    // PD2 - настройка через МАКРОСЫ

    // Конфигурируем на вход
    GPIOD_PIN2_MODER_CLEAR();

    // Настройка типа выхода push pull
    GPIOD_PIN2_OTYPER_CLEAR();

    // Настройка скорости работы - средняя
    GPIOD_PIN2_OSPEEDR_MED();

    // Подтяжка резистора - pull up
    GPIOD_PIN2_PUPDR_PULLUP();

    // PG2 и PE2 - настройка через CMSIS (библиотека)

    // Конфигурируем на вход
    CLEAR_BIT(GPIOG->MODER, GPIO_MODER_MODE2);
    CLEAR_BIT(GPIOE->MODER, GPIO_MODER_MODE2);

    // Настройка типа выхода push pull
    CLEAR_BIT(GPIOG->OTYPER, GPIO_OTYPER_OT_2);
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_2);

    // Настройка скорости работы - средняя
    SET_BIT(GPIOG->OSPEEDR, GPIO_OSPEEDER_OSPEEDR2_0);
    SET_BIT(GPIOE->OSPEEDR, GPIO_OSPEEDER_OSPEEDR2_0);

    // Подтяжка резисторов - pull up
    SET_BIT(GPIOG->PUPDR, GPIO_PUPDR_PUPDR2_0);
    SET_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR2_0);

    // Светодиоды на плате (GPIOB) CMSIS
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODE7 | GPIO_MODER_MODE0 | GPIO_MODER_MODE14);
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE7_0 | GPIO_MODER_MODE0_0 | GPIO_MODER_MODE14_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT7_Msk | GPIO_OTYPER_OT0_Msk | GPIO_OTYPER_OT14_Msk);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR7_0 | GPIO_OSPEEDER_OSPEEDR0_0 | GPIO_OSPEEDER_OSPEEDR14_0);
}

// OPTIMIZE: возможно стоит добавить верхнюю границу для обработки зажатой кнопки

// принимает аргументом название шины порта и бит для чтения
// возвращает true если кнопка нажата и else в остальных случаях
bool button_bounce_read(GPIO_TypeDef *GPIOx, uint32_t idr_mask)
{
    bool press = false;
    int32_t bounce = 0;
    do
    {
        if (READ_BIT(GPIOx->IDR, idr_mask) == 0)
        {
            bounce++;
            btn1_st = true;
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
void changeConf(GPIO_TypeDef *GPIO0, GPIO_TypeDef *GPIO1, uint32_t PORT0_bit, uint32_t PORT1_bit, uint32_t PORT0_LIGTH, uint32_t PORT1_LIGTH, bool noRepeat)
{
    if (noRepeat == true)
    {
        // сброс
        CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODE2);
        CLEAR_BIT(GPIOG->MODER, GPIO_MODER_MODE2);
        CLEAR_BIT(GPIOE->MODER, GPIO_MODER_MODE2);

        // смена режима
        SET_BIT(GPIO0->MODER, PORT0_bit);
        SET_BIT(GPIO1->MODER, PORT1_bit);

        // свет
        SET_BIT(GPIO0->BSRR, PORT0_LIGTH);
        SET_BIT(GPIO1->BSRR, PORT1_LIGTH);
    }
}

// NOTE: принимает аргументами:тип порта на котором находится кнопка (GPIOx), бит для чтения с порта GPIOx,
// бит для записи в GPIOB, бит для очистки бита в GPIOB
void button_light_led(GPIO_TypeDef *GPIOx, uint32_t IDR_bit, uint32_t BSRR_BS, uint32_t BSRR_BR)
{
    if (READ_BIT(GPIOx->IDR, IDR_bit) == 0)
    {
        SET_BIT(GPIOB->BSRR, BSRR_BS);
        if (BSRR_BS == GPIO_BSRR_BS7_Msk)
        {
            blue_led = true;
        }
        else if (BSRR_BS == GPIO_BSRR_BS14_Msk)
        {
            red_led = true;
        }
        else
        {
            yellow_led = true;
        }
    }
    else
    {
        if (BSRR_BS == GPIO_BSRR_BS7_Msk)
        {
            blue_led = false;
        }
        else if (BSRR_BS == GPIO_BSRR_BS14_Msk)
        {
            red_led = false;
        }
        else
        {
            yellow_led = false;
        }
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
    if (button_bounce_read(GPIOC, GPIO_IDR_IDR_11) == true)
    {
        bright++;
    }
    if (bright > 3)
    {
        bright = 1;
    }

    if (button_bounce_read(GPIOD, GPIO_IDR_IDR_2) == true)
    {
        counter_press8++;
    }
    if (button_bounce_read(GPIOG, GPIO_IDR_IDR_2) == true)
    {
        counter_press9++;
    }
    if (button_bounce_read(GPIOE, GPIO_IDR_IDR_2) == true)
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
    // NOTE: используемые порты: PC11; PD2; PG2; PE2;
    static int8_t counter_press = 0, old_counter_press = 0;
    if (button_bounce_read(GPIOC, GPIO_IDR_IDR_11) == true)
    {
        counter_press++;
    }
    btn1_st = false;
    // NOTE: функция смены конфигурации вызывается только один раз

    if (counter_press == 1)
    {
        changeConf(GPIOG, GPIOE, GPIO_MODER_MODER2_0, GPIO_MODER_MODER2_0, GPIO_BSRR_BS2, GPIO_BSRR_BS2, (old_counter_press != counter_press));
        btn2_st = false;
        btn3_st = true;
        btn4_st = true;
        old_counter_press = counter_press;
        // 10 на вход, остальные на выход
        button_light_led(GPIOD, GPIO_IDR_IDR_2, GPIO_BSRR_BS7_Msk, GPIO_BSRR_BR7_Msk);
    }
    else if (counter_press == 2)
    {

        changeConf(GPIOD, GPIOE, GPIO_MODER_MODER2_0, GPIO_MODER_MODER2_0, GPIO_BSRR_BS2, GPIO_BSRR_BS2, (old_counter_press != counter_press));
        btn2_st = true;
        btn3_st = false;
        btn4_st = true;
        old_counter_press = counter_press;
        // 9 на вход остальные на выход
        button_light_led(GPIOG, GPIO_IDR_IDR_2, GPIO_BSRR_BS14_Msk, GPIO_BSRR_BR14_Msk);
    }
    else if (counter_press == 3)
    {

        changeConf(GPIOD, GPIOG, GPIO_MODER_MODER2_0, GPIO_MODER_MODER2_0, GPIO_BSRR_BS2, GPIO_BSRR_BS2, (old_counter_press != counter_press));
        btn2_st = true;
        btn3_st = true;
        btn4_st = false;
        old_counter_press = counter_press;
        // 8 на вход остальные на выход
        button_light_led(GPIOE, GPIO_IDR_IDR_2, GPIO_BSRR_BS0_Msk, GPIO_BSRR_BR0_Msk);
    }
    else if (counter_press == 4)
    {
        // все порты в режим выхода
        if (old_counter_press != counter_press)
        {
            SET_BIT(GPIOE->MODER, GPIO_MODER_MODER2_0);
            btn2_st = true;
            btn3_st = true;
            btn4_st = true;
            old_counter_press = counter_press;
        }
    }
    else if ((counter_press == 5) | (counter_press == 0))
    {
        // все порты в режим входа
        if (old_counter_press != counter_press)
        {
            CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODE2);
            CLEAR_BIT(GPIOG->MODER, GPIO_MODER_MODE2);
            CLEAR_BIT(GPIOE->MODER, GPIO_MODER_MODE2);
            btn2_st = false;
            btn3_st = false;
            btn4_st = false;
            old_counter_press = counter_press;
        }
        button_light_led(GPIOD, GPIO_IDR_IDR_2, GPIO_BSRR_BS7_Msk, GPIO_BSRR_BR7_Msk);
        button_light_led(GPIOG, GPIO_IDR_IDR_2, GPIO_BSRR_BS14_Msk, GPIO_BSRR_BR14_Msk);
        button_light_led(GPIOE, GPIO_IDR_IDR_2, GPIO_BSRR_BS0_Msk, GPIO_BSRR_BR0_Msk);
    }
    else if (counter_press > 5)
    {
        counter_press = 1;
    }
}
