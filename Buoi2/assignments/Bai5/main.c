#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018)
#define RCC_APB1ENR (*(volatile uint32_t *)0x4002101C)
#define GPIOA_CRL   (*(volatile uint32_t *)0x40010800)
#define GPIOA_CRH   (*(volatile uint32_t *)0x40010804)

#define TIM2_CR1    (*(volatile uint32_t *)0x40000000)
#define TIM2_EGR    (*(volatile uint32_t *)0x40000014)
#define TIM2_CCMR1  (*(volatile uint32_t *)0x40000018)
#define TIM2_CCER   (*(volatile uint32_t *)0x40000020)
#define TIM2_PSC    (*(volatile uint32_t *)0x40000028)
#define TIM2_ARR    (*(volatile uint32_t *)0x4000002C)
#define TIM2_CCR1   (*(volatile uint32_t *)0x40000034)

#define USART1_SR   (*(volatile uint32_t *)0x40013800)
#define USART1_DR   (*(volatile uint32_t *)0x40013804)
#define USART1_BRR  (*(volatile uint32_t *)0x40013808)
#define USART1_CR1  (*(volatile uint32_t *)0x4001380C)
#define NVIC_ISER1  (*(volatile uint32_t *)0xE000E104)

#define COMMAND_SIZE 32U

static volatile char command[COMMAND_SIZE];
static volatile uint8_t command_index;
static volatile uint8_t command_ready;
static uint8_t led_on;
static uint8_t pwm_percent = 50U;

static void uart_send_char(char value)
{
    while ((USART1_SR & (1U << 7)) == 0U) {
    }
    USART1_DR = (uint8_t)value;
}

static void uart_send_string(const char *text)
{
    while (*text != '\0') {
        uart_send_char(*text++);
    }
}

static void uart_send_percent(uint8_t value)
{
    if (value == 100U) {
        uart_send_string("100");
    } else if (value >= 10U) {
        uart_send_char((char)('0' + value / 10U));
        uart_send_char((char)('0' + value % 10U));
    } else {
        uart_send_char((char)('0' + value));
    }
}

static void uart1_init(void)
{
    RCC_APB2ENR |= (1U << 2) | (1U << 14);
    /* PA9: USART1 TX. PA10: USART1 RX. */
    GPIOA_CRH = (GPIOA_CRH & ~(0xFFU << 4)) | (0xBU << 4) | (0x4U << 8);
    USART1_BRR = 0x341U; /* 9600 baud with HSI 8 MHz. */
    USART1_CR1 = (1U << 13) | (1U << 5) | (1U << 3) | (1U << 2);
    /* USART1 IRQ 37 is bit 5 in ISER1. */
    NVIC_ISER1 |= (1U << 5);
}

static void pwm_set(uint8_t percent)
{
    /* LED is active-low: 3V3 -> LED -> resistor -> PA0. */
    TIM2_CCR1 = (uint32_t)(100U - percent) * 10U;
}

static void pwm_init(void)
{
    RCC_APB2ENR |= (1U << 2);
    RCC_APB1ENR |= (1U << 0);
    /* PA0: TIM2 CH1 PWM output. */
    GPIOA_CRL = (GPIOA_CRL & ~0xFU) | 0xBU;
    TIM2_PSC = 7U;
    TIM2_ARR = 999U;
    TIM2_CCMR1 = (6U << 4) | (1U << 3);
    TIM2_CCER = 1U;
    pwm_set(0U);
    TIM2_CR1 = (1U << 7);
    TIM2_EGR = 1U;
    TIM2_CR1 |= 1U;
}

static uint8_t command_is(const char *text)
{
    uint8_t index = 0U;

    while (text[index] != '\0') {
        if (command[index] != text[index]) {
            return 0U;
        }
        ++index;
    }
    return command[index] == '\0';
}

static uint8_t get_pwm_percent(uint8_t *percent)
{
    uint8_t index = 4U;
    uint16_t value = 0U;

    if (command[0] != 'P' || command[1] != 'W' ||
        command[2] != 'M' || command[3] != ':') {
        return 0U;
    }
    if (command[index] < '0' || command[index] > '9') {
        return 0U;
    }
    while (command[index] >= '0' && command[index] <= '9') {
        value = value * 10U + (uint16_t)(command[index] - '0');
        if (value > 100U) {
            return 0U;
        }
        ++index;
    }
    if (command[index] != '%' || command[index + 1U] != '\0') {
        return 0U;
    }
    *percent = (uint8_t)value;
    return 1U;
}

static void process_command(void)
{
    uint8_t value;

    if (command_is("ON")) {
        led_on = 1U;
        pwm_set(pwm_percent);
        uart_send_string("LED ON\n\r");
    } else if (command_is("OFF")) {
        led_on = 0U;
        pwm_set(0U);
        uart_send_string("LED OFF\n\r");
    } else if (get_pwm_percent(&value)) {
        pwm_percent = value;
        if (led_on != 0U) {
            pwm_set(pwm_percent);
        }
        uart_send_string("PWM: ");
        uart_send_percent(pwm_percent);
        uart_send_string("%\n\r");
    } else if (command_is("Status")) {
        uart_send_string((led_on != 0U) ? "LED: ON, PWM: " : "LED: OFF, PWM: ");
        uart_send_percent(pwm_percent);
        uart_send_string("%\n\r");
    } else {
        uart_send_string("Unknown command\n\r");
    }
}

void USART1_IRQHandler(void)
{
    char received;

    if ((USART1_SR & (1U << 5)) == 0U) {
        return;
    }
    received = (char)(USART1_DR & 0xFFU);
    if (command_ready != 0U) {
        return;
    }
    if (received == '!') {
        command[command_index] = '\0';
        command_ready = 1U;
    } else if (command_index < COMMAND_SIZE - 1U) {
        command[command_index++] = received;
    }
}

int main(void)
{
    pwm_init();
    uart1_init();
    uart_send_string("System ready\n\r");

    while (1) {
        if (command_ready != 0U) {
            process_command();
            command_index = 0U;
            command_ready = 0U;
        }
    }
}
