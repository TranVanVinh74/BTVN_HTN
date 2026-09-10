#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sdata, _edata, _sidata;
extern uint32_t _sbss, _ebss;

int main(void);

void Reset_Handler(void) {
    // Copy dữ liệu .data từ Flash sang SRAM
    uint32_t *pSrc = &_sidata;
    uint32_t *pDst = &_sdata;
    while (pDst < &_edata) {
        *pDst++ = *pSrc++;
    }

    // Khởi tạo vùng .bss về 0
    pDst = &_sbss;
    while (pDst < &_ebss) {
        *pDst++ = 0;
    }

    // Nhảy vào hàm main
    main();

    while (1);
}

void Default_Handler(void) {
    while (1);
}

// Bảng Vector Table
__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))((uint32_t)&_estack),
    Reset_Handler,
    Default_Handler,
    Default_Handler
};
