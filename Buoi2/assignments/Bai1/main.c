#include <stdint.h>

#define RCC_APB2ENR     (*(volatile uint32_t *)0x40021018)
#define GPIOA_CRH       (*(volatile uint32_t *)0x40010804)
#define USART1_SR       (*(volatile uint32_t *)0x40013800)
#define USART1_DR       (*(volatile uint32_t *)0x40013804)
#define USART1_BRR      (*(volatile uint32_t *)0x40013808)
#define USART1_CR1      (*(volatile uint32_t *)0x4001380C)

#define RX_BUF_SIZE     256
#define PREFIX_INFO     "ELE1415-01-NHOM-02"

static void uart1_init(void) {
  RCC_APB2ENR |= (1 << 2) | (1 << 14);
  GPIOA_CRH &= ~(0xFF << 4);
  GPIOA_CRH |= (0x0B << 4) | (0x04 << 8);
  USART1_BRR = 0x341; /* 9600 baud, PCLK2 = HSI = 8 MHz. */
  USART1_CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}

static void uart1_send_char(char c) {
  while (!(USART1_SR & (1 << 7)));
  USART1_DR = (uint8_t)c;
}

static char uart1_receive(void) {
  while (!(USART1_SR & (1 << 5)));
  return (char)(USART1_DR & 0xFF);
}

static void uart1_send_string(const char *str) {
  while (*str) {
    uart1_send_char(*str++);
  }
}

int main(void) {
  uart1_init();
  char rx_buf[RX_BUF_SIZE];
  uint16_t idx = 0;
  while(1) {
    char i = uart1_receive();
    if (i == '!') {
      rx_buf[idx] = '\0';
      uart1_send_string(PREFIX_INFO ": ");
      uart1_send_string(rx_buf);
      uart1_send_string("\n\r");

      idx = 0;
    }
    else if (idx < RX_BUF_SIZE - 1) {
      rx_buf[idx++] = i;
    }
  }
}
