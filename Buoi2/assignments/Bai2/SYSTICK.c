#include "SYSTICK.h"

// Định nghĩa địa chỉ thanh ghi SysTick (Core Cortex-M3)
#define SYSTICK_CTRL (*((volatile uint32_t *)0xE000E010))
#define SYSTICK_LOAD (*((volatile uint32_t *)0xE000E014))
#define SYSTICK_VAL  (*((volatile uint32_t *)0xE000E018))

volatile uint32_t systick_ms = 0;

void SysTick_Init(void)
{
    SYSTICK_LOAD = 8000 - 1;   
    SYSTICK_VAL  = 0;

    SYSTICK_CTRL = (1<<2) |      // Clock = AHB
                   (1<<1) |      // Enable interrupt
                   (1<<0);       // Enable SysTick
}

void SysTick_Handler(void)
{
    systick_ms++;
}

uint32_t millis(void)
{
    return systick_ms;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = millis();
    while(millis() - start < ms);
}