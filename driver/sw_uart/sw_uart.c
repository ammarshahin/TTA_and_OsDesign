/******************************************************************************/
/* @File            : Sw_uart.c                                    			  */
/*                                                                            */
/* @Designed by     : Ammar Shahin                         @Date : 4-7-2020   */
/*                                                                            */
/* @Designed by     : Ammar Shahin                         @Date : 4-7-2020   */
/*                                                                            */
/* @Coding language : C                                                       */
/*                                                                            */
/******************************************************************************/

/************************************************************************/
/*                       Files Included                                 */
/************************************************************************/
#include "sw_uart.h"
#include "mcal.h"

#define debug 0

#if debug
#include "utils.h"
#include "uart.h"
uint8_t buff[10] = {0};
#endif

/************************************************************************/
/*                        private Macros                                 */
/************************************************************************/

/*
#define Tx_pinWriteHigh() SET_BIT(Tx_DataOutputReg, Tx_pin)
#define Tx_pinWriteLow() CLR_BIT(Tx_DataOutputReg, Tx_pin)
#define Rx_pinRead() GET_BIT(Rx_DataInputReg, Rx_pin)
*/

#define Tx_pinWriteHigh() mcal_gpio_pinState_set(&px_gpioTx, MCAL_GPIO_HIGH)
#define Tx_pinWriteLow() mcal_gpio_pinState_set(&px_gpioTx, MCAL_GPIO_LOW)
#define Rx_pinRead() mcal_gpio_pinState_get(&px_gpioRx)
/************************************************************************/
/*                        private Enum Typedefs                          */
/************************************************************************/
typedef enum
{
    STOP_BIT = 1,
    START_BIT,
    DATA_SYMBOLE
} tEnum_uartInnerState;

typedef enum
{
    NO_OPERATION = 1,
    SEND_OPERATION,
    RECEIVE_OPERATION
} tEnum_swUartOperation;
/************************************************************************/
/*                           Private Structs Typedefs                    */
/************************************************************************/

/************************************************************************/
/*                   GLOBAL STATIC(Private) VARIABLES                   */
/************************************************************************/
volatile static uint16_t gu16_uartTimer;

volatile static tEnum_swUartOperation gEnum_uartCurrentOperation;

static uint8_t gu8_uartDataTx;
static uint8_t gu8_uartDataRx;

static bool_t gbool_TxCompleteFlag;
static bool_t gbool_RxCompleteFlag;

mcal_gpio_t px_gpioTx = {MCAL_GPIO_PORTD, MCAL_GPIO_PIN4, MCAL_GPIO_OUTPUT, MCAL_GPIO_HIGH, MCAL_GPIO_EXTINT_DISABLE, MCAL_GPIO_EXTINT_NONE};
mcal_gpio_t px_gpioRx = {MCAL_GPIO_PORTD, MCAL_GPIO_PIN3, MCAL_GPIO_INPUT, MCAL_GPIO_NONE, MCAL_GPIO_EXTINT_ENABLE, MCAL_GPIO_EXTINT_FALLING_EDGE};

/************************************************************************/
/*                     Private Functions prototypes                     */
/************************************************************************/
static void Sw_uart_SendOperation_StateProcess(void);
static void Sw_uart_ReceiveOperation_StateProcess(void);

/************************************************************************/
/*                           APIs Implementation                         */
/************************************************************************/
/**
 * Function : sw_uart_init
 * Description: This function is to initialize the Software UART Module.
 * @return nothing
 */
void sw_uart_init(void)
{
    /* Set the Tx_pin to be o/p Pin */
    // SET_BIT(Tx_ControlReg, Tx_pin);
    /* Set the Tx pin to be Idle high */

    mcal_gpio_pin_init(&px_gpioTx);

    /* Set the Rx_pin to be i/p Pin */
    /* Initialize Rx External interrupt pin To the Falling Edge*/
    /* set the Rx_pin to be high pull up */
    // CLR_BIT(Rx_ControlReg, Rx_pin);
    // SET_BIT(Rx_DataOutputReg, Rx_pin);
    // MCUCR |= (1 << ISC01);
    // GICR |= (1 << INT0);
    mcal_gpio_pin_init(&px_gpioRx);

    /* Initialize the uart global variables */
    gu16_uartTimer = 0;

    gu8_uartDataTx = 0;
    gu8_uartDataRx = 0;

    gEnum_uartCurrentOperation = NO_OPERATION;

    gbool_TxCompleteFlag = TRUE;
    gbool_RxCompleteFlag = FALSE;
}

/**
 * Function : InverterCom_Update
 * Description: This function is to update the Communication Module
 * @return nothing
 */
void sw_uart_update(void *args)
{
    gu16_uartTimer++;
    if (gu16_uartTimer < SW_UART_INTERNAL_UPDATE_PERIOD_MS)
    {
        return;
    }

    gu16_uartTimer = 0;
    /* Main sw_uart State machine */
    switch (gEnum_uartCurrentOperation)
    {
    case NO_OPERATION:
        Tx_pinWriteHigh();
        break;
    case SEND_OPERATION:
        Sw_uart_SendOperation_StateProcess();
        break;
    case RECEIVE_OPERATION:
        Sw_uart_ReceiveOperation_StateProcess();
        break;
    default:
        break;
    }
}

/**
 * Function : sw_uart_send
 * Description: This function is used to send a byte through the uart
 * @param data is the data to be sent
 * @return nothing
 */
void sw_uart_send(uint8_t data)
{
    if (gEnum_uartCurrentOperation == NO_OPERATION)
    {
        gu8_uartDataTx = data;
        gEnum_uartCurrentOperation = SEND_OPERATION;
    }
    else
    {
        // LOGErr(Sw_uart,DOR); // Data Overrun
    }
}

/**
 * Function : sw_uart_read
 * Description: This function is used to read a byte through the uart
 * @return the data received
 */
uint8_t sw_uart_read(void)
{
    return gu8_uartDataRx;
}

/**
 * Function : sw_uart_TxComplete
 * Description: This function is used to read the status of the transmission compelation
 * @return the status of the transmission compelation [TRUE, or FALSE]
 */
bool_t sw_uart_TxComplete(void)
{
    return gbool_TxCompleteFlag;
}

/**
 * Function : sw_uart_RxComplete
 * Description: This function is used to read the status of the reception compelation
 * @return the status of the reception compelation [TRUE, or FALSE]
 */
bool_t sw_uart_RxComplete(void)
{
    return gbool_RxCompleteFlag;
}

/**
 * Function : sw_uart_TxFlagClr
 * Description: This function is used to clear the TxComplete flag
 * @return Nothing
 */
void sw_uart_TxFlagClr(void)
{
    gbool_TxCompleteFlag = FALSE;
}

/**
 * Function : sw_uart_RxFlagClr
 * Description: This function is used to clear the RxComplete flag
 * @return Nothing
 */
void sw_uart_RxFlagClr(void)
{
    gbool_RxCompleteFlag = FALSE;
}

/************************************************************************/
/*                 Private Functions Implementation                     */
/************************************************************************/
/**
 * Function : Sw_uart_SendOperation_StateProcess
 * Description: This function is used to process the inner state machine of the sending operation
 * @return nothing
 */
static void Sw_uart_SendOperation_StateProcess(void)
{
    static uint8_t u8_iteratorIndex = 0;
    static tEnum_uartInnerState gEnum_uartSendingState = START_BIT;
    uint8_t data = 0;

    switch (gEnum_uartSendingState)
    {
    case START_BIT:
        Tx_pinWriteLow();
        gEnum_uartSendingState = DATA_SYMBOLE;
        break;
    case DATA_SYMBOLE:

        data = ((gu8_uartDataTx >> u8_iteratorIndex) & 0x01);   // Assuming LSB will be Sent first
        if (data == 1)
        {
            Tx_pinWriteHigh();
        }
        else if (data == 0)
        {
            Tx_pinWriteLow();
        }
        else
        {
            // Do Nothing
        }

        u8_iteratorIndex++;
        if (u8_iteratorIndex == SYMBOLE_LENGTH)   // index == 8
        {
            u8_iteratorIndex = 0;
            gEnum_uartSendingState = STOP_BIT;
        }
        break;
    case STOP_BIT:
        Tx_pinWriteHigh();
        gEnum_uartSendingState = START_BIT;
        gEnum_uartCurrentOperation = NO_OPERATION;
        gbool_TxCompleteFlag = TRUE;
        u8_iteratorIndex = 0;
        break;
    }
}

/**
 * Function : Sw_uart_ReceiveOperation_StateProcess
 * Description: This function is used to process the inner state machine of the RECEIVE operation
 * @return nothing
 */
static void Sw_uart_ReceiveOperation_StateProcess(void)
{
    static uint8_t u8_iteratorIndex = 0;
    static tEnum_uartInnerState uartReceivingState = START_BIT;

    switch (uartReceivingState)
    {
    case START_BIT:
        uartReceivingState = DATA_SYMBOLE;
        gu8_uartDataRx = 0;
        break;
    case DATA_SYMBOLE:
        gu8_uartDataRx |= (Rx_pinRead() << u8_iteratorIndex);   // Assuming LSB will be received first
        u8_iteratorIndex++;
        if (u8_iteratorIndex == SYMBOLE_LENGTH)   // u8_iteratorIndex == 8
        {
            u8_iteratorIndex = 0;
            uartReceivingState = STOP_BIT;
        }
        break;
    case STOP_BIT:
        gEnum_uartCurrentOperation = NO_OPERATION;
        gbool_RxCompleteFlag = TRUE;
        uartReceivingState = START_BIT;
        u8_iteratorIndex = 0;
        break;
    }
}

/************************************************************************/
/*                      External ISR Implementation                     */
/************************************************************************/

/**
 * Function : sw_uart_isr_handler
 * Description: This function is called in the External Interrupt that will fire when Falling edge occurs on the Rx Pin
 * @return nothing
 */
void sw_uart_isr_handler(void)   // On Rx pin
{
    if (gEnum_uartCurrentOperation == NO_OPERATION)
    {
        gEnum_uartCurrentOperation = RECEIVE_OPERATION;
        /* debug */
        gu16_uartTimer = (uint8_t)SW_UART_INTERNAL_UPDATE_PERIOD_MS / 2;
    }
    else
    {
        // LOGErr(InverterCom,FRAME Error);
    }
}