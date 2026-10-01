#include <stdint.h>

#define RCC_APB2ENR    (*(volatile uint32_t *)0x40021018UL)
#define GPIOC_CRH      (*(volatile uint32_t *)0x40011004UL)
#define GPIOC_BSRR     (*(volatile uint32_t *)0x40011010UL)
#define GPIOC_BRR      (*(volatile uint32_t *)0x40011014UL)

#define LED_PIN        13U
#define LED_PIN_MASK   (1UL << LED_PIN)

static void delay(volatile uint32_t count)
{
    while (count-- != 0U) {
        __asm volatile ("nop");
    }
}

int main(void)
{
    RCC_APB2ENR |= (1UL << 4);

    GPIOC_CRH &= ~(0xFUL << 20U);
    GPIOC_CRH |= (0x2UL << 20U);

    while (1) {
        GPIOC_BRR = LED_PIN_MASK;
        delay(500000U);
        GPIOC_BSRR = LED_PIN_MASK;
        delay(500000U);
    }
}
