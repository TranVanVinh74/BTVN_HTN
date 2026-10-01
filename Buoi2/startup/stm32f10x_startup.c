#include <stdint.h>

#define SRAM_START 0x20000000U
#define SRAM_SIZE (20U * 1024U)
#define SRAM_END ((SRAM_START) + (SRAM_SIZE))

#define STACK_START SRAM_END

extern uint8_t _sidata;
extern uint8_t _sdata;
extern uint8_t _edata;
extern uint8_t _sbss;
extern uint8_t _ebss;

int main(void);

void Reset_Handler(void) __attribute__((noreturn));
void Default_Handler(void);

void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

void WWDG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TAMPER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USB_HP_CAN1_TX_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void USB_LP_CAN1_RX0_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler(void)
	__attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USBWakeUp_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

const uintptr_t vectors[] __attribute__((used, section(".isr_vector"))) = {
	STACK_START,
	(uintptr_t)&Reset_Handler,
	(uintptr_t)&NMI_Handler,
	(uintptr_t)&HardFault_Handler,
	(uintptr_t)&MemManage_Handler,
	(uintptr_t)&BusFault_Handler,
	(uintptr_t)&UsageFault_Handler,
	0,
	0,
	0,
	0,
	(uintptr_t)&SVC_Handler,
	(uintptr_t)&DebugMon_Handler,
	0,
	(uintptr_t)&PendSV_Handler,
	(uintptr_t)&SysTick_Handler,
	(uintptr_t)&WWDG_IRQHandler,
	(uintptr_t)&PVD_IRQHandler,
	(uintptr_t)&TAMPER_IRQHandler,
	(uintptr_t)&RTC_IRQHandler,
	(uintptr_t)&FLASH_IRQHandler,
	(uintptr_t)&RCC_IRQHandler,
	(uintptr_t)&EXTI0_IRQHandler,
	(uintptr_t)&EXTI1_IRQHandler,
	(uintptr_t)&EXTI2_IRQHandler,
	(uintptr_t)&EXTI3_IRQHandler,
	(uintptr_t)&EXTI4_IRQHandler,
	(uintptr_t)&DMA1_Channel1_IRQHandler,
	(uintptr_t)&DMA1_Channel2_IRQHandler,
	(uintptr_t)&DMA1_Channel3_IRQHandler,
	(uintptr_t)&DMA1_Channel4_IRQHandler,
	(uintptr_t)&DMA1_Channel5_IRQHandler,
	(uintptr_t)&DMA1_Channel6_IRQHandler,
	(uintptr_t)&DMA1_Channel7_IRQHandler,
	(uintptr_t)&ADC1_2_IRQHandler,
	(uintptr_t)&USB_HP_CAN1_TX_IRQHandler,
	(uintptr_t)&USB_LP_CAN1_RX0_IRQHandler,
	(uintptr_t)&CAN1_RX1_IRQHandler,
	(uintptr_t)&CAN1_SCE_IRQHandler,
	(uintptr_t)&EXTI9_5_IRQHandler,
	(uintptr_t)&TIM1_BRK_IRQHandler,
	(uintptr_t)&TIM1_UP_IRQHandler,
	(uintptr_t)&TIM1_TRG_COM_IRQHandler,
	(uintptr_t)&TIM1_CC_IRQHandler,
	(uintptr_t)&TIM2_IRQHandler,
	(uintptr_t)&TIM3_IRQHandler,
	(uintptr_t)&TIM4_IRQHandler,
	(uintptr_t)&I2C1_EV_IRQHandler,
	(uintptr_t)&I2C1_ER_IRQHandler,
	(uintptr_t)&I2C2_EV_IRQHandler,
	(uintptr_t)&I2C2_ER_IRQHandler,
	(uintptr_t)&SPI1_IRQHandler,
	(uintptr_t)&SPI2_IRQHandler,
	(uintptr_t)&USART1_IRQHandler,
	(uintptr_t)&USART2_IRQHandler,
	(uintptr_t)&USART3_IRQHandler,
	(uintptr_t)&EXTI15_10_IRQHandler,
	(uintptr_t)&RTC_Alarm_IRQHandler,
	(uintptr_t)&USBWakeUp_IRQHandler};

void Default_Handler(void)
{
	while (1) {
	}
}

void Reset_Handler(void)
{
	uint8_t *pDst = &_sdata;
	uint8_t *pSrc = &_sidata;

	while (pDst < &_edata) {
		*pDst++ = *pSrc++;
	}

	pDst = &_sbss;
	while (pDst < &_ebss) {
		*pDst++ = 0;
	}

	main();

	while (1) {
	}
}
