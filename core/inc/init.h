#include <stdint.h>
#include <stdbool.h>
#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"

void RCC_Init(void);
void GPIO_Init(void);
void ITR_Init(void);
void SysTick_Init(void);

// Полупериоды для частот мерцания светодиодов в мс
#define FREQUENCY1 1250 // Частота 0.4 Гц
#define FREQUENCY2 455  // Частота 1.1 Гц
#define FREQUENCY3 263  // Частота 1.9 Гц

// Структура для управления светодиодом
typedef struct
{
    uint8_t number;               // номер светодиода
    uint32_t toggle_time_ms;      // момент времени последнего переключения состояния
    uint32_t delay_time_ms;       // время задержки для мерцания
    bool led_state;               // true = включен, false = выключен
    uint32_t current_btn_hold_2s; // количество нажатий с удержанием в 2 с
} Led;

// Функции для работы со светодиодами
void Led_init(Led *led, uint8_t num);
void Led_on(Led *led);
void Led_off(Led *led);
void Led_flicker(Led *led);
void Led_set_delay_time_ms(Led *led);





