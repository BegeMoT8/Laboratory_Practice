#include <stdint.h>
#include <stdbool.h>
#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"
#include <it_handlers.h>
volatile uint32_t GlobalTickCount; // Глобальный счетчик тиков SysTick
volatile uint8_t btn_count = 0;    // Счетчик коротких нажатий
volatile uint8_t btn_hold_2s = 0;  // Счетчик удержаний 2 сек
volatile uint8_t btn_hold_4s = 0;  // Счетчик удержаний 4 сек
#define delayTime_MS 50
#define shortHoldTime_MS 2000
#define longHoldTime_MS 4000
#define Led_Count 6

/**
 * @brief Обработчик прерывания по EXTI линии 13 (кнопка на PC13)
 * @details Комбинированный подход борьбы с дребезгом:
 * - Временной фильтр 50 мс
 * - Отслеживание состояния кнопки
 * - Измерение длительности удержания
 * - Определение типа нажатия: короткое / 2 сек / 4 сек
 */
void EXTI15_10_IRQHandler(void)
{
    static uint32_t last_interrupt_time = 0;
    static uint32_t press_start_time = 0;
    static bool button_pressed = false;

    uint32_t current_time = GlobalTickCount;

    // игнорируем прерывания в течение 50 мс после последнего
    if ((current_time - last_interrupt_time) < delayTime_MS)
    {
        SET_BIT(EXTI->PR, EXTI_PR_PR13);
        return;
    }

    last_interrupt_time = current_time;

    // 1 = нажата (подтянута к VDD), 0 = отпущена (подтянута к GND)
    bool current_state = READ_BIT(GPIOC->IDR, GPIO_IDR_ID13);

    if (current_state && !button_pressed) // Нажатие фронт
    {
        button_pressed = true;
        press_start_time = current_time;
    }
    else if (!current_state && button_pressed) // Отпускание спад
    {
        button_pressed = false;
        uint32_t press_duration = current_time - press_start_time;

        // Определяем тип нажатия
        if (press_duration < shortHoldTime_MS) // Короткое нажатие (< 2 сек)
        {
            btn_count++; // Функция 1: переключение светодиодов
            if (btn_count > Led_Count)
            {
                btn_count = 0; // Сброс после 6-го светодиода
            }
        }
        else if (press_duration < longHoldTime_MS) // Удержание 2-4 сек
        {
            btn_hold_2s++; // Функция 2: изменение частоты мерцания
        }
        else // Удержание >= 4 сек
        {
            btn_hold_4s++; // Функция 3: изменение режима работы
        }
    }

    SET_BIT(EXTI->PR, EXTI_PR_PR13); // сброс флага прерывания
}

/**
 * @brief Обработчик прерывания системного таймера SysTick
 * @details Увеличивает глобальный счетчик тиков на 1 при каждом срабатывании прерывания.
 */
void SysTick_Handler(void)
{
    GlobalTickCount++;//Одна итерация - 1 мс
}
