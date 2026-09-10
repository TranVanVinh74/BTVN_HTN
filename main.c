#include <stdint.h>

#define RCC_BASE      0x40021000
#define GPIOC_BASE    0x40011000

#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define GPIOC_CRH     (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_ODR     (*(volatile uint32_t *)(GPIOC_BASE + 0x0C))

void delay(volatile uint32_t count) {
    while (count--) {
        __asm__("nop");
    }
}

int main(void) {
    // 1. Bật Clock cho Port C (Bit 4)
    RCC_APB2ENR |= (1 << 4);

    // 2. Cấu hình PC13: Output Push-Pull, tốc độ 2MHz (Mode: 10, CNF: 00)
    GPIOC_CRH &= ~(0xF << 20); // Xóa cấu hình cũ của Pin 13
    GPIOC_CRH |= (0x2 << 20);  // Đặt Mode 2MHz Push-Pull

    while (1) {
        GPIOC_ODR ^= (1 << 13); // Đảo trạng thái chân PC13
        delay(500000);
    }
    return 0;
}
