#include "ADC.h"
#include "SYSTICK.h"

// RCC & GPIO
#define RCC_APB2ENR       (*((volatile uint32_t *)0x40021018))
#define RCC_CFGR          (*((volatile uint32_t *)0x40021004))
#define GPIOA_CRL         (*((volatile uint32_t *)0x40010800))

// ADC1
#define ADC1_SR           (*((volatile uint32_t *)0x40012400))
#define ADC1_CR1          (*((volatile uint32_t *)0x40012404))
#define ADC1_CR2          (*((volatile uint32_t *)0x40012408))
#define ADC1_SMPR2        (*((volatile uint32_t *)0x40012410))
#define ADC1_SQR1         (*((volatile uint32_t *)0x4001242C))
#define ADC1_SQR3         (*((volatile uint32_t *)0x40012434))
#define ADC1_DR           (*((volatile uint32_t *)0x4001244C))

void ADC1_Init(void) {
    // 1. Cấp xung cho GPIOA và ADC1
    RCC_APB2ENR |= (1 << 2) | (1 << 9);

    // 2. Cấu hình Prescaler cho ADC (PCLK2 / 6 = 8MHz / 6 = 1.33MHz < 14MHz)
    RCC_CFGR |= (2 << 14);

    // 3. Cấu hình PA0 làm Analog Input (MODE=00, CNF=00)
    GPIOA_CRL &= ~(0x0F << 0);

    // 4. Cấu hình Sample time cho Channel 0: 55.5 cycles (SMPR2 bits [2:0] = 101)
    ADC1_SMPR2 |= (5 << 0);

    // 5. Cấu hình 1 chuyển đổi trong Regular Sequence (L[3:0] = 0000 ở SQR1)
    ADC1_SQR1 &= ~(0x0F << 20);

    // 6. Chọn Channel 0 là chuyển đổi đầu tiên (SQ1[4:0] = 0 ở SQR3)
    ADC1_SQR3 &= ~(0x1F << 0);

    // 7. Bật nguồn ADC (ADON = 1)
    ADC1_CR2 |= (1 << 0);
    delay_ms(1); // Chờ ADC ổn định

    // 8. Hiệu chuẩn ADC (Calibration)
    ADC1_CR2 |= (1 << 2);              // RSTCAL: Reset calibration
    while (ADC1_CR2 & (1 << 2));       // Chờ reset calibration xong
    ADC1_CR2 |= (1 << 3);              // CAL: Bắt đầu hiệu chuẩn
    while (ADC1_CR2 & (1 << 3));       // Chờ hiệu chuẩn xong
}

uint16_t ADC1_Read(void) {
    ADC1_CR2 |= (1 << 0);              // Bật cờ ADON để bắt đầu chuyển đổi (SWSTART)
    while (!(ADC1_SR & (1 << 1)));     // Chờ cờ EOC (End of Conversion)
    return (uint16_t)ADC1_DR;          // Đọc kết quả
}