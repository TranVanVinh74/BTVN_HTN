#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t *)(0x40021018))

#define GPIOA_CRL (*(volatile uint32_t *)(0x40010800))
#define GPIOA_IDR (*(volatile uint32_t *)(0x40010808))
#define GPIOA_ODR (*(volatile uint32_t *)(0x4001080C))

#define GPIOC_CRH (*(volatile uint32_t *)(0x40011004))
#define GPIOC_ODR (*(volatile uint32_t *)(0x4001100C))

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
    // Enable GPIOA and GPIOC
    RCC_APB2ENR |= (1 << 2) | (1 << 4);

    // PA7: Input Pull-up
    GPIOA_CRL &= ~(0xF << 28);
    GPIOA_CRL |= (0x8 << 28);
    GPIOA_ODR |= (1 << 7);

    // PC13: Output Push-Pull, 50 MHz
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |= (0x3 << 20);

    // LED OFF initially
    GPIOC_ODR |= (1 << 13);

    while (1)
    {
        // Nhấn nút
        if ((GPIOA_IDR & (1 << 7)) == 0)
        {
            delay_ms(20);

            // Xác nhận nút vẫn được nhấn
            if ((GPIOA_IDR & (1 << 7)) == 0)
            {
                // Chờ nhả nút
                while ((GPIOA_IDR & (1 << 7)) == 0);

                delay_ms(20);

                // đảo led
                GPIOC_ODR ^= (1 << 13);
            }
        }
    }
}
