#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)(0x40021018))

#define GPIOA_CRL     (*(volatile uint32_t *)(0x40010800))
#define GPIOA_CRH     (*(volatile uint32_t *)(0x40010804))
#define GPIOA_IDR     (*(volatile uint32_t *)(0x40010808))
#define GPIOA_ODR     (*(volatile uint32_t *)(0x4001080C))

int main(void)
{
    uint32_t input;

    // Enable GPIOA clock
    RCC_APB2ENR |= (1 << 2);

    // PA0 - PA4: Input Pull-up
    GPIOA_CRL &= ~(0xFFFFF);
    GPIOA_CRL |= 0x88888;

    // PA8 - PA12: Output Push-Pull, 50 MHz
    GPIOA_CRH &= ~(0xFFFFF);
    GPIOA_CRH |= 0x33333;

    // Select Pull-up for PA0 - PA4
    GPIOA_ODR |= 0x1F;

    while (1)
    {
        input = (~GPIOA_IDR) & 0x1F;

        // Preserve Pull-up selection bits while updating LEDs.
        GPIOA_ODR &= ~(0x1F << 8);
        GPIOA_ODR |= input << 8;
    }
}
