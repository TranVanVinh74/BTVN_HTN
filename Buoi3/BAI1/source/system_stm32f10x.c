#include "stm32f10x.h"
#include "system_stm32f10x.h"

/* STM32F103 starts from the internal 8 MHz HSI clock after reset. */
uint32_t SystemCoreClock = 8000000U;

void SystemInit(void)
{
    /* Keep the reset clock configuration; application code may set it with SPL. */
}

void SystemCoreClockUpdate(void)
{
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    SystemCoreClock = clocks.HCLK_Frequency;
}
