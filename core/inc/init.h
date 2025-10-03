#include <stdint.h>
#include <stdbool.h>
#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"

void GPIO_Ini(void);
bool button_bounce_read(uint32_t idr_mask);
void changeConf(uint32_t PORT0_bit, uint32_t PORT1_bit, uint32_t PORT0_LIGTH, uint32_t PORT1_LIGTH, bool noRepeat);