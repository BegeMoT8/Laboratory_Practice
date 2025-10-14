#include <stdint.h>
#include <stdbool.h>
#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"

void GPIO_Ini(void);
bool button_bounce_read(uint32_t idr_mask);
void changeConf(uint32_t PORT0_bit, uint32_t PORT1_bit, uint32_t PORT0_LIGTH, uint32_t PORT1_LIGTH, bool noRepeat);
void button_light_led(GPIO_TypeDef *GPIOx, uint32_t IDR_bit, uint32_t BSRR_BS, uint32_t BSRR_BR);
void mode_light(uint8_t ratio, uint32_t BSRR_BSx, uint32_t BSRR_BRx);
void changing_brightness();
void main_task();

// NOTE: макросы для настройки режима работы порта вручную

// RCC + AHB1ENR
#define RCC_GPIO_EN (*(uint32_t *)(0x40023800UL + 0x30UL))
// бит в GPIOB
#define RCC_GPIOB_EN 0x02UL
//(адрес GPIOB + адрес MODER) для настройки режима порта (00 input; 01 output)
#define GPIOB_MODER (*(uint32_t *)(0x40020400UL + 0x00UL))
//(адрес GPIOB + адрес OTYPER) тип выхода (push-pull(0) \ open-Drain(1))
#define GPIOB_OTYPER (*(uint32_t *)(0x40020400UL + 0x04UL))
//(адрес GPIOB + адрес OSPEEDR) скорость работы пинов порта
#define GPIOB_OSPEEDR (*(uint32_t *)(0x40020400UL + 0x08UL))
// для регистра MODER на output для 0 порта (синий светодиод) |0 бит единичка|
#define GPIOB_MODE_PIN0_OUT 0x1UL
// настраиваем тип выхода на push-pull (0)
#define GPIOB_OTYPER_PIN0_PP 0x0000UL