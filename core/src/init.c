#include "init.h"
/**
 * @brief Инициализация тактирования микроконтроллера STM32F4xx
 * @details Настройка тактирования на работу с внешним кварцевым резонатором 8 МГц и
 * получение системной частоты 180 МГц с использованием PLL.
 * @note Необходима вызвать в начале main
 */
void RCC_Init(void)
{
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U); // очистка битов HSITRIM и установка
    CLEAR_REG(RCC->CFGR);                       // очистка (выбор источника системной частоты)
    while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET)
        ;

    // очистка битов PLLON, HSEON, CSSON, HSEBYP
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET)
        ;
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET)
        ;
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

    // Настройка главного регистра RCC_CR
    SET_BIT(RCC->CR, RCC_CR_HSEON); // Запускаем внешний кварцевый резонатор
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET)
        ;                              // Ждём пока он запустится
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); // отключение шунтирования для избежания наводок
    SET_BIT(RCC->CR, RCC_CR_CSSON);    // отслеживание ошибки в работе HSE

    // Настройка регистра RCC_PLLCFGR
    CLEAR_REG(RCC->PLLCFGR);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE); // Выбираем в качестве источника PLL внешний кварцевый резонатор (вероятно 8 МГц)

    // Настройка множителей и делителей PLL
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_2); // Выставляем предделитель входной частоты PLL на 4
    //-------------------------------------------------
    // N = 60 ||0011 1100 000000
    // N = 80 ||0101 0000 000000
    // N = 90 ||0101 1010 000000

    // MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_2 |
    // RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5); //Настраиваем умножение частоты, полученной после деления (частоты VCO) на 60

    // MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_6); //Настраиваем умножение частоты, полученной после деления (частоты VCO) на 80

    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_1 | RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_6); // Настраиваем умножение частоты, полученной после деления (частоты VCO) на 90

    // MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_2 |
    // RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7); //Настраиваем умножение частоты, полученной после деления (частоты VCO) на х180

    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk); // Настраиваем предделитель получившейся частоты после умножения. Иными словами, получаем итоговую частоту PLL
    // MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk, RCC_PLLCFGR_PLLP_0); //Делим на 4 (90 МГц)

    SET_BIT(RCC->CR, RCC_CR_PLLON); // Запустим PLL
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY))
        ; // Ждём запуска PLL

    // Настройка регистра RCC_CFGR
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);        // Выбираем PLL в качестве System Clock
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);   // Предделитель AHB, без делителя (180 МГц)
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4); // Предделитель APВ1, делим на 4 (45 МГц)
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2); // Предделитель APВ2, делим на 2 (90 МГц)

    // MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO2PRE, RCC_CFGR_MCO2PRE_Msk); //Предделитель на выходе MCO2 (PC9) = 5 (180МГц/5 = 36МГц)
    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO2PRE, RCC_CFGR_MCO2PRE_2); // деление на 2

    CLEAR_BIT(RCC->CFGR, RCC_CFGR_MCO2); // Выбираем в качестве источника MCO2 PLL

    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS); // Настройка задержки работы памяти FLASH на 5 тактов
}

/**
 * @brief настройка GPIO пинов микроконтроллера
 * @details Настройка пинов PC11; PD2; PG2; PE2;
 * @note Необходима вызвать в начале main
 */
void GPIO_Init(void)
{
    // NOTE: используемые порты: PC11; PD2; PG2; PE2;

    // включаем тактирование на пины GPIOC/D/E/G/B
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOBEN);

    SET_BIT(GPIOC->MODER, GPIO_MODER_MODER9_1);           // Настраиваем пин на альтернативный режим
    SET_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED9_Msk);    // Настраиваем пин на максимальную скорость работы
    MODIFY_REG(GPIOC->AFR[1], GPIO_AFRH_AFSEL9_Msk, 0x0); // Выбираем тип альтернативной функции – Выход MCO2
}
/**
 * @brief Инициализация прерывания по кнопке на пине PC13
 * @details Настройка прерывания EXTI13 на пине PC13 по фронту и спаду сигнала.
 * Установка приоритета прерывания в 0.
 * @note Необходима вызвать в начале main
 */
void ITR_Init(void)
{
    // Настройка регистра SYSCFG_EXTICR4
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);                                        // включение тактирования SYSCFG
    MODIFY_REG(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI13_Msk, SYSCFG_EXTICR4_EXTI13_PC); // Настройка мультиплексора на вывод линии прерывания EXTI13 на PC13
    // Настройка регистров EXTI
    SET_BIT(EXTI->IMR, EXTI_IMR_MR13);   // Маскирования линии прерывания
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR13); // Отслеживание по фронту
    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR13); // Отслеживание по спаду
    // Настройка регистров NVIC
    NVIC_SetPriority(EXTI15_10_IRQn,
                     NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0)); // установка приоритета прерывания
    NVIC_EnableIRQ(EXTI15_10_IRQn);                                          // разрешения прерывания
}
