#include "init.h"

void GPIO_Ini(){
    // *(uint32_t *)(0x40023800UL + 0x30UL) |= 0x06;   // Включение тактирования портов GPIOB и GPIOC
    // *(uint32_t *)(0x40020400UL + 0x00UL) |= 0x4000; // Настройка работы 7-го пина GPIOB в режиме вывода сигнала (Output mode)
    // *(uint32_t *)(0x40020400UL + 0x04UL) |= 0x00;   // Настройка на PushPull работу 7-го пина GPIOB (Output Push-Pull)
    // *(uint32_t *)(0x40020400UL + 0x08UL) |= 0x4000; // Настройка скорости работы 7-го пина GPIOB на среднюю
    // *(uint32_t *)(0x40020400UL + 0x0CUL) |= 0x00;   // Отключение PU/PD резисторов для 7-го пина GPIOB
   
    // RCC_GPIO_EN |= RCC_GPIOB_EN + RCC_GPIOC_EN;
    // GPIOB_MODER |= GPIOB_MODE_PIN7_OUT;
    // GPIOB_OTYPER |= GPIOB_OTYPER_PIN7_PP;
    // GPIOB_OSPEEDR |= GPIOB_OSPEED_PIN7_MID;
    // GPIOB_PUPDR |= GPIOB_PUPDR_PIN7_NOPUPD;

    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE7_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_7);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR7_0);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR7_0);
    
}