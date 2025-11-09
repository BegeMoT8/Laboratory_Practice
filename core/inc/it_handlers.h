void EXTI15_10_IRQHandler(void);
void SysTick_Handler(void);

// Переменные для отслеживания нажатий кнопки
extern volatile uint8_t btn_count;    // Счетчик коротких нажатий
extern volatile uint8_t btn_hold_2s;  // Счетчик удержаний 2 сек
extern volatile uint8_t btn_hold_4s;  // Счетчик удержаний 4 сек