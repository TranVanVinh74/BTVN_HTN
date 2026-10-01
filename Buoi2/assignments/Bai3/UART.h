#ifndef __UART_H
#define __UART_H

#include <stdint.h>

void UART1_Init(void);
void UART1_SendChar(char c);
void UART1_SendString(const char *str);
void Send_Voltage(uint16_t adc_val);

#endif