#include <stdint.h>
#include <stdbool.h>
#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"

void RCC_Init(void);
void GPIO_Init(void);
void ITR_Init(void);
void SysTick_Init(void);
void led_on(uint8_t count);
void led_off(void);