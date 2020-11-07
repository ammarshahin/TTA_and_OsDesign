#ifndef BSP_H
#define BSP_H

#define F_CPU 8000000UL

#define PLL_FACTOR (8)

#define ATMEGA328P 1525

// #define MCU (TMPM370)
#define MCU (ATMEGA328P)

#if (MCU == TMPM370)
#include "bsp_tmpm370.h"
#elif (MCU == ATMEGA328P)
#include "bsp_atmega328P.h"
#endif

#define CRITICAL_SECTION_START()   //__disable_irq()
#define CRITICAL_SECTION_END()     //__enable_irq()

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void INTRX0_IRQHandler(void);
void INTRX1_IRQHandler(void);
void INTRX2_IRQHandler(void);
void INTRX3_IRQHandler(void);

void bsp_clockConfig_set(void);
void bsp_interruptPriorities_set(void);

#endif
