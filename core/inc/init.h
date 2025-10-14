#include <stdint.h>
#include <stdbool.h>
#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"

void GPIO_Ini(void);
bool button_bounce_read(GPIO_TypeDef *GPIOx, uint32_t idr_mask);
// void changeConf(GPIO_TypeDef *GPIO0, GPIO_TypeDef *GPIO1, uint32_t PORT0_bit, uint32_t PORT1_bit, uint32_t PORT0_LIGTH, uint32_t PORT1_LIGTH, bool noRepeat)
void button_light_led(GPIO_TypeDef *GPIOx, uint32_t IDR_bit, uint32_t BSRR_BS, uint32_t BSRR_BR);
void mode_light(uint8_t ratio, uint32_t BSRR_BSx, uint32_t BSRR_BRx);
void changing_brightness();
void main_task();
void changeConf(GPIO_TypeDef *GPIO0, GPIO_TypeDef *GPIO1, uint32_t PORT0_bit, uint32_t PORT1_bit, uint32_t PORT0_LIGTH, uint32_t PORT1_LIGTH, bool noRepeat);
// NOTE: макросы для настройки режима работы порта PD2

#define GPIOD_BASE_ADDR 0x40020C00UL
#define GPIOD_MODER_ADDR (GPIOD_BASE_ADDR + 0x00UL)
#define GPIOD_OTYPER_ADDR (GPIOD_BASE_ADDR + 0x04UL)
#define GPIOD_OSPEEDR_ADDR (GPIOD_BASE_ADDR + 0x08UL)
#define GPIOD_PUPDR_ADDR (GPIOD_BASE_ADDR + 0x0CUL)

#define GPIOD_MODER (*(uint32_t *)(GPIOD_MODER_ADDR))
#define GPIOD_OTYPER (*(uint32_t *)(GPIOD_OTYPER_ADDR))
#define GPIOD_OSPEEDR (*(uint32_t *)(GPIOD_OSPEEDR_ADDR))
#define GPIOD_PUPDR (*(uint32_t *)(GPIOD_PUPDR_ADDR))

// Макросы для конфигурации пина 2 порта D
#define GPIOD_PIN2_MODER_CLEAR() (GPIOD_MODER &= ~(0x3UL << 4))
#define GPIOD_PIN2_OTYPER_CLEAR() (GPIOD_OTYPER &= ~(0x1UL << 2))
#define GPIOD_PIN2_OSPEEDR_MED() (GPIOD_OSPEEDR |= (0x1UL << 4))
#define GPIOD_PIN2_PUPDR_PULLUP() (GPIOD_PUPDR |= (0x1UL << 4))