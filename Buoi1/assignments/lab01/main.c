#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)(0x40021018))
#define GPIOC_CRH     (*(volatile uint32_t *)(0x40011004))
#define GPIOC_ODR     (*(volatile uint32_t *)(0x4001100C))

void delay_ms(uint32_t ms)
{
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 0x500; j++);
    }
}

int main(void)
{
    // Enable GPIOC clock
    RCC_APB2ENR |= (1 << 4);

    // PC13: Output Push-Pull, 50 MHz
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |=  (0x3 << 20);

    while (1)
    {
        // LED ON
        GPIOC_ODR &= ~(1 << 13);
        delay_ms(1000);

        // LED OFF
        GPIOC_ODR |= (1 << 13);
        delay_ms(1000);
    }
}
