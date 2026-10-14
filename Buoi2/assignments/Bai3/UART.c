#include "UART.h"

// RCC & GPIO
#define RCC_APB2ENR       (*((volatile uint32_t *)0x40021018))
#define GPIOA_CRH         (*((volatile uint32_t *)0x40010804))

// USART1
#define USART1_SR         (*((volatile uint32_t *)0x40013800))
#define USART1_DR         (*((volatile uint32_t *)0x40013804))
#define USART1_BRR        (*((volatile uint32_t *)0x40013808))
#define USART1_CR1        (*((volatile uint32_t *)0x4001380C))

void UART1_Init(void) {
    // 1. Bật Clock cho GPIOA và USART1
    RCC_APB2ENR |= (1 << 2) | (1 << 14);

    // 2. Cấu hình PA9 (USART1_TX): Alternate function output Push-pull, max speed 50MHz
    GPIOA_CRH &= ~(0x0F << 4);
    GPIOA_CRH |=  (0x0B << 4); // Mode=11, CNF=10

    // 3. Cấu hình Baudrate 9600 (xung nội 8MHz: BRR = 8000000 / 9600 = 0x341)
    USART1_BRR = 0x341;

    // 4. Bật USART1 và bộ truyền TX
    USART1_CR1 |= (1 << 13) | (1 << 3); // UE = 1, TE = 1
}

void UART1_SendChar(char c) {
    while (!(USART1_SR & (1 << 7))); // Chờ cờ TXE
    USART1_DR = (c & 0xFF);
}

void UART1_SendString(const char *str) {
    while (*str) {
        UART1_SendChar(*str++);
    }
}

void Send_Voltage(uint16_t adc_val) {
    uint32_t volt_mv = ((uint32_t)adc_val * 3300) / 4095;
    uint32_t v_int = volt_mv / 1000;
    uint32_t v_dec = volt_mv % 1000;

    UART1_SendString("ADC: ");
    
    char buf[5];
    buf[0] = (adc_val / 1000) + '0';
    buf[1] = ((adc_val % 1000) / 100) + '0';
    buf[2] = ((adc_val % 100) / 10) + '0';
    buf[3] = (adc_val % 10) + '0';
    buf[4] = '\0';
    UART1_SendString(buf);

    UART1_SendString(" | Voltage: ");
    UART1_SendChar(v_int + '0');
    UART1_SendChar('.');
    UART1_SendChar((v_dec / 100) + '0');
    UART1_SendChar(((v_dec % 100) / 10) + '0');
    UART1_SendChar((v_dec % 10) + '0');
    UART1_SendString(" V\r\n");
}