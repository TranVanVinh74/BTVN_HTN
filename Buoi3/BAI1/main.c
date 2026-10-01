#include "stm32f10x.h"
#include "ssd1306.h"

// Monochrome test image (128x64 pixels = 1024 bytes).
const uint8_t image_mono_128x64[1024] = {
    [0 ... 511] = 0xAA,
    [512 ... 1023] = 0x55
};

int main(void) {
    SSD1306_Init();
    SSD1306_Clear();

    // Draw the complete monochrome image buffer on the OLED.
    SSD1306_DrawBitmap(image_mono_128x64);

    while (1) {
        // The SSD1306 keeps displaying its internal buffer without refreshes.
    }
}