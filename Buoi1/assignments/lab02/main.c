#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)(0x40021018))
#define GPIOA_CRL     (*(volatile uint32_t *)(0x40010800))
#define GPIOA_ODR     (*(volatile uint32_t *)(0x4001080C))

void delay_ms(uint32_t ms)
{
    volatile uint32_t i, j;

    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 0x500; j++);
    }
}

int main(void)
{
    // Bật clock GPIOA
    RCC_APB2ENR |= (1 << 2);

    // PA0 - PA7: Output Push-Pull, 50 MHz
    GPIOA_CRL = 0x33333333;

    while (1)
    {
        // Chạy từ trái sang phải
        for (int i = 0; i < 8; i++)
        {
            GPIOA_ODR = (1 << i);
            delay_ms(200);
        }

        // Chạy từ phải sang trái
        for (int i = 6; i >= 0; i--)
        {
            GPIOA_ODR = (1 << i);
            delay_ms(200);
        }
    }
}
