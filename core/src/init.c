#include "init.h"
// КНОПКИ подключены на PC8 PC9 PC10 PC11
// управление режимом выхода порта осуществляется при помощи кнопки без светодиода. изначально при помощи удержания кнопок к которым парраллельно подключен
// светодиод можно зажигать светодиоды на плате при помощи удержания этих кнопок.
// 1 при нажатии кнопки без светодиода: первая кнопка переходит в режим выход (больше не управляет светодиодом на плате), остальные работают
//  2 при нажатии кнопки без светодиода: вторая кнопка переходит в режим выход (больше не управляет светодиодом на плате), остальные работают
//   3 при нажатии кнопки без светодиода: третья кнопка переходит в режим выход (больше не управляет светодиодом на плате), остальные работают
// далее по заданию
void GPIO_Ini()
{
    // включаем тактирование на пины GPIOCEN GPIOBEN
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN);
    // конфигурируем все пины на вход
    CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE10 | GPIO_MODER_MODE11);
    // TODO: добавь зеленый
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODE0 | GPIO_MODER_MODE7 | GPIO_MODER_MODE14);
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE0_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER14_0);
    // тип выхода push pull
    CLEAR_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10);
    // скорость работы - средняя
    SET_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR8_0 | GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR10_0 | GPIO_OSPEEDER_OSPEEDR11_0);
    // подтяжка резистора pull up (если убрать подтяжку кнопка станет сенсорной (⊙_⊙;))
    SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0);
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
// HACK: можно сделать функцию от двух переменных
void changeConf(uint32_t PORT0_bit, uint32_t PORT1_bit, uint32_t PORT0_LIGTH, uint32_t PORT1_LIGTH, bool noRepeat)
{
    if (noRepeat == true)
    {
        CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODE8 | GPIO_MODER_MODE9 | GPIO_MODER_MODE10);
        SET_BIT(GPIOC->MODER, PORT0_bit | PORT1_bit);
        SET_BIT(GPIOC->BSRR, PORT0_LIGTH | PORT1_LIGTH);
    }
}