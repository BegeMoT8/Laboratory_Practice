// вариант №3
#include <init.h>
#include <it_handlers.h>

uint8_t LedState;

int main(void)
{
    RCC_Init();
    GPIO_Init();
    ITR_Init();
    while (1)
    {
        if (LedState)
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
        else
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7);
    }
}