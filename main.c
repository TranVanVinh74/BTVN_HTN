#include <stdint.h>
#include "SYSTICK.h"

// Định nghĩa địa chỉ thanh ghi RCC và GPIOA (STM32F1)
#define RCC_APB2ENR (*((volatile uint32_t *)0x40021018))
#define GPIOA_CRL   (*((volatile uint32_t *)0x40010800))
#define GPIOA_ODR   (*((volatile uint32_t *)0x4001080C))

int main(void)
{
    uint32_t t1=0, t2=0, t3=0;

    // Cấp xung nhịp cho GPIOA
    RCC_APB2ENR |= (1<<2); 

    // Cấu hình PA0, PA1, PA2 làm Output Push-Pull, max speed 50MHz
    GPIOA_CRL &= ~(uint32_t)(0xFFF);
    GPIOA_CRL |= (0x333);
    
    SysTick_Init();
    
    while (1)
    {
        if(millis() - t1 > 10000){
            GPIOA_ODR ^= (1<<0);
            t1 = millis();
        }
        if(millis() - t2 > 1000){
            GPIOA_ODR ^= (1<<1);
            t2 = millis();
        }
        if(millis() - t3 > 100){
            GPIOA_ODR ^= (1<<2);
            t3 = millis();
        }
    }
}