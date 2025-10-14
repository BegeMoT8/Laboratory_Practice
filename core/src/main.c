// вариант №3
#include <init.h>

int main(void)
{
    GPIO_Ini();

    while (1)
    {
        // NOTE: однвременный запуск двух задач не поддерживается!

        //changing_brightness(); // дополнительное задание
        main_task(); // основное задание
    }
}