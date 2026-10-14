#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

/* SPL I2C_Send7bitAddress expects the 7-bit address shifted left once. */
#define SSD1306_I2C_ADDR (0x3C << 1)

void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_DrawBitmap(const uint8_t *bitmap);

#endif
