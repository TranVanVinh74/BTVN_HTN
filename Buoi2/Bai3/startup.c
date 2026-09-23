#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sdata, _edata, _sidata;
extern uint32_t _sbss, _ebss;

// Khai báo các hàm từ main.c và SYSTICK.c
extern int main(void);
extern void SysTick_Handler(void);

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

// Gắn Default_Handler cho các ngoại lệ hệ thống khác nếu chưa được viết
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));

// Bảng Vector Table cho Cortex-M3
__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack), // 0: Top of Stack
    Reset_Handler,              // 1: Reset Handler
    NMI_Handler,                // 2: NMI Handler
    HardFault_Handler,          // 3: Hard Fault Handler
    MemManage_Handler,          // 4: MPU Fault Handler
    BusFault_Handler,           // 5: Bus Fault Handler
    UsageFault_Handler,         // 6: Usage Fault Handler
    0,                          // 7: Reserved
    0,                          // 8: Reserved
    0,                          // 9: Reserved
    0,                          // 10: Reserved
    SVC_Handler,                // 11: SVCall Handler
    DebugMon_Handler,           // 12: Debug Monitor Handler
    0,                          // 13: Reserved
    PendSV_Handler,             // 14: PendSV Handler
    SysTick_Handler             // 15: SysTick Handler
};