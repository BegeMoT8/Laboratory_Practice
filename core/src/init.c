#include "init.h"
/**
 * @brief Инициализация тактирования микроконтроллера STM32F4xx
 * @details Настройка тактирования на работу с внешним кварцевым резонатором 8 МГц и
 * получение системной частоты 180 МГц с использованием PLL.
 * @note Необходима вызвать в начале main
 */
void RCC_Init(void)
{
    /* Предварительная очистка регистров RCC */
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);
    CLEAR_REG(RCC->CFGR);
    while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET)
        ;
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET)
        ;
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET)
        ;
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
    /* Настройка главного регистра RCC */
    SET_BIT(RCC->CR, RCC_CR_HSEON); // Запускаем внешний кварцевый резонатор
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET)
        ;                              // Ждём пока он запустится
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); // Сбросим бит байпаса в 0, если вдруг там что-то лежит
    SET_BIT(RCC->CR, RCC_CR_CSSON);    // Запустим Clock detector
    /* Настройка регистров PLL
    * Предварительная очистка регистра
    * В качестве источника тактирования для PLL выбирается HSE
    * Мы сначала делим входную частоту (HSE) на 4 (получаем 2 МГц), затем умножаем на 180 и
    снова делим на 2, таким образом получаем 180МГц
    * Включаем работу PLL
    */
    CLEAR_REG(RCC->PLLCFGR);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_2); // Выставляем предделитель входной частоты PLL на 4 
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7);
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk); // Настраиваем предделитель получившейся частоты после умножения.Иными словами, получаем итоговую частоту PLL
                                               SET_BIT(RCC->CR, RCC_CR_PLLON); // Запустим PLL
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY))
        ; // Ждём запуска PLL
    /* Настройка основных конфигураций RCC
     * В качетсве системных часов выбираем выход PLL
     * Настраиваем предделители шин AHB и APB
     * Настраиваем выходы MCO1 и MCO2 для внешней оценки настроенной системы тактирования
     */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL); // Выбираем PLL в качестве System Clock
        MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);    // Предделитель AHB, без делителя
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4);      // Предделитель APВ1, делим на4 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2);    // Предделитель APВ2, делим на2 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO2PRE, RCC_CFGR_MCO2PRE_Msk); // Предделитель на выходеMCO2(PC9) = 5 
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_MCO2);               // Настраиваем на выход MCO2 - System clock
    /* Настройка задержки внутренней памяти
    * Выставление битов LATENCY регистра FLASH_ACR в позицию 5SW (6 CPU cycles).
    * Данная настройка необходима при увеличении системной частоты тактирования свыше 20 МГц.
    Таблица 12 RM0090
    */
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS);
}

/**
 * @brief настройка GPIO пинов микроконтроллера
 * @details Настройка пинов PC11; PD2; PG2; PE2;
 * @note Необходима вызвать в начале main
 */
void GPIO_Init(void)
{
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
void SysTick_Init(void)
{
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);  // На всякий случай, предварительно, выключим счётчик
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);   // Разрешаем прерывание по системному таймеру
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk); // Источник тактирования будет идти из AHB без деления
    MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk,
               179999 << SysTick_LOAD_RELOAD_Pos); // Значение с которого начинается счёт, эквивалентное 1 кГц (частота AHB поделить на это число плюс 1 180000000/(179999+1)=1000 Гц)
    MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk,
               179999 << SysTick_VAL_CURRENT_Pos);   // Очистка поля
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); // Включим счётчик
}