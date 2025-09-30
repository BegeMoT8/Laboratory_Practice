// включение светодиода по нажатию кнопки
#include <init.h>
int main(void)
{
    GPIO_Ini();
    while (1)
    {
        if (READ_BIT(GPIOC->IDR, GPIO_IDR_IDR_13) != 0){
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
        }
        else {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7); // Установка единицы в 7-ой бит регистра ODR
        }
    }
    // while (1)
    // {
    //     if (READ_BIT(GPIOC_IDR, GPIOC_IDR_PIN13) != 0){
    //         SET_BIT(GPIOB_BSRR, GPIOB_BSRR_PIN7_SET);
    //     }
    //     else {
    //         SET_BIT(GPIOB_BSRR, GPIOB_BSRR_PIN7_RESET); // Установка единицы в 7-ой бит регистра ODR
    //     }
    // }
}