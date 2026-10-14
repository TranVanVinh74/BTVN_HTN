#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stdint.h>

extern volatile uint32_t systick_ms;

void SysTick_Init(void);
uint32_t millis(void);
void delay_ms(uint32_t ms);
void SysTick_Handler(void);

#endif