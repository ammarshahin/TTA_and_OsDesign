#include "mcal.h"
#include "bsp.h"
#include "config.h"

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1)
    {
        printf("hard fault...........\n\r");
    }
#if 1

    static volatile uint32_t gIsContinue = 0;
    /* For development FW, MCU run into the while loop when the hardfault occurred.                           */
    /* 1. Stack Checking                                                                                      */
    /*    When a hard fault exception occurs, MCU push following register into the stack (main or process     */
    /*    stack). Confirm R13(SP) value in the Register Window and typing it to the Memory Windows, you can   */
    /*    check following register, especially the PC value (indicate the last instruction before hard fault).*/
    /*    SP + 0x00    0x04    0x08    0x0C    0x10    0x14    0x18    0x1C                                   */
    /*           R0      R1      R2      R3     R12      LR      PC    xPSR                                   */
    while (gIsContinue == 0)
    {
    }
    /* 2. Step Out to Find the Clue                                                                           */
    /*    Change the variable "gIsContinue" to any other value than zero in a Local or Watch Window, then     */
    /*    step out the HardFault_Handler to reach the first instruction after the instruction which caused    */
    /*    the hard fault.                                                                                     */
    /*--------------------------------------------------------------------------------------------------------*/

#else

    /*--------------------------------------------------------------------------------------------------------*/
    /* For production FW, you shall consider to reboot the system when hardfault occurred.                    */
    /*--------------------------------------------------------------------------------------------------------*/
    //NVIC_SystemReset();

#endif
}

void MemManage_Handler(void)
{
    printf("here1...........\n\r");
    while (1)
        ;
}

void BusFault_Handler(void)
{
    printf("here2...........\n\r");
    while (1)
        ;
}

void UsageFault_Handler(void)
{
    printf("here3...........\n\r");
    while (1)
        ;
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    //os_tick_update(&os_scheduler_update, OS_TICK_PERIOD_MS);
}
//
//void INTRX0_IRQHandler(void)
//{
    //volatile UART_Err err;
//
    //err = UART_GetErrState(UART0);
    //if (UART_NO_ERR == err)
    //{
        ////RxBuffer1[RxCounter1++] = (uint8_t)UART_GetRxData(UART1);
        //// uart_recv_int(UART_0);
    //}
    ////uart_byte_put(UART_0, (uint8_t)UART_GetRxData(UART0));
    //uart_recv_int(UART_0);
//}
//
//void INTRX1_IRQHandler(void)
//{
    //volatile UART_Err err;
//
    //err = UART_GetErrState(UART1);
    //if (UART_NO_ERR == err)
    //{
        ////RxBuffer1[RxCounter1++] = (uint8_t)UART_GetRxData(UART1);
        //// uart_recv_int(UART_1);
    //}
    ////uart_byte_put(UART_1, (uint8_t)UART_GetRxData(UART1));
    //uart_recv_int(UART_1);
//}
//
//void INTRX2_IRQHandler(void)
//{
    //volatile UART_Err err;
//
    //err = UART_GetErrState(UART2);
    //if (UART_NO_ERR == err)
    //{
        ////RxBuffer1[RxCounter1++] = (uint8_t)UART_GetRxData(UART2);
        //// uart_recv_int(UART_2);
    //}
    ////uart_byte_put(UART_2, (uint8_t)UART_GetRxData(UART2));
    //uart_recv_int(UART_2);
//}

/*********************************************************************************************/

void bsp_clockConfig_set(void)
{
    mcal_cg_system_set();
}

void bsp_interruptPriorities_set(void)
{
#if 0
    NVIC_SetPriority(INTA_IRQn, 1);
    NVIC_SetPriority(SysTick_IRQn, 2);
#endif
}

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
    while (1)
    {
    }
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
    __assert_func(file, line, NULL, failedexpr);
}
