#include <stdint.h>
#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"

void GPIO_Ini(void);

















//ВАРИАНТ БЕЗ БИБЛИОТЕКИ CMIS

// //инициализация пинов
// void GPIO_Ini();

// //(адрес GPIOC + адрес IDR |input data register|);
// #define GPIOC_IDR (*(uint32_t*)(0x40020800UL + 0x10UL))
// //0x2000UL - 13 пин кнопки
// #define GPIOC_IDR_PIN13 0x002000UL
// //проверка high сигнала на n порту (кнопка)
// #define READ_BIT(REG, BIT)   (REG & BIT)

// //(адрес GPIOB + адрес GPIOx_BSRR) регистр записи\сброса битов
// #define GPIOB_BSRR (*(uint32_t*)(0x40020400UL + 0x18UL))
// // 1 в 7 бит\led
// #define GPIOB_BSRR_PIN7_SET 0x000080UL
// // 0 в 7 бит\led
// #define GPIOB_BSRR_PIN7_RESET 0x800000UL
// //установка бита в n регистр с k позицией
// #define SET_BIT(REG, BIT)   (REG |= BIT)


// //адрес регистра тактирования всех GPIO (адрес RCC + адрес RCC AHB1)
// #define RCC_GPIO_EN (*(uint32_t*)(0x40023800UL + 0x30UL))

// //(адрес GPIOB + адрес MODER) для настройки режима порта (00 input; 01 output)
// #define GPIOB_MODER (*(uint32_t*)(0x40020400UL + 0x00UL))

// //(адрес GPIOB + адрес OTYPER) тип выхода (push-pull(0) \ open-Drain(1))
// #define GPIOB_OTYPER (*(uint32_t*)(0x40020400UL + 0x04UL))

// //(адрес GPIOB + адрес OSPEEDR) скорость работы пинов порта
// #define GPIOB_OSPEEDR (*(uint32_t*)(0x40020400UL + 0x08UL))

// //(адрес GPIOB + адрес PUPDR) настройка подтяжки пина (pull-up/pull-down register) |01- pullup; 10-pulldown|
// #define GPIOB_PUPDR (*(uint32_t*)(0x40020400UL + 0x0CUL))



// //значения для записи в регистры

// //второй бит для разблокировки порта GPIOB (...10)
// #define RCC_GPIOB_EN 0x02UL
// //третий бит для разблокировки порта GPIOB (...100)
// #define RCC_GPIOC_EN 0x04UL

// //настройка выходного порта
// //для регистра MODER на output для 7 порта (синий светодиод) |14 бит единичка❤ (счет с нуля)|
// #define GPIOB_MODE_PIN7_OUT 0x4000UL
// //настраиваем тип выхода на push-pull (0)
// #define GPIOB_OTYPER_PIN7_PP 0x0000UL
// //pull-up/pull-down |00-нет никакой подтяжки|
// #define GPIOB_PUPDR_PIN7_NOPUPD 0x0000UL

// //|14 бит единичка❤ (счет с нуля)| medium speed: 7 порт
// #define GPIOB_OSPEED_PIN7_MID 0x4000UL