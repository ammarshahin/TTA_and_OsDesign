/******************************************************************************/
/* @File            : Sw_uart.h                                    			  */
/*                                                                            */
/* @Designed by     : Ammar Shahin                         @Date : 4-7-2020   */
/*                                                                            */
/* @Designed by     : Ammar Shahin                         @Date : 4-7-2020   */
/*                                                                            */
/* @Coding language : C                                                       */
/*                                                                            */
/* @COPYRIGHT 2020 El-ARABY Research and development center.                  */
/* all rights reserved                                                        */
/******************************************************************************/
#ifndef _SW_UART_H__
#define _SW_UART_H__

/************************************************************************/
/*                       Files Included                                 */
/************************************************************************/
#include "std_types.h"
#include "utils.h"
#include "mcal.h"
#include "config.h"

/************************************************************************/
/*                        Public Macros                                 */
/************************************************************************/
//#define UART_BAUDRATE 75
#define SYMBOLE_LENGTH 8   // 8 bit == 1 Byte
#define PARITY_OPERATION 0 // [ 0 >> No Parity, 1 >> +ve Parity, 2 >> -ve parity]
#define STOP_BIT_NUMBER 1
#define START_BIT_NUMBER 1

#define Tx_pin //PIND1
#define Rx_pin //PIND2

#define Tx_ControlReg //DDRD
#define Rx_ControlReg //DDRD

#define Tx_DataOutputReg //PORTD

#define Rx_DataOutputReg //PORTD
#define Rx_DataInputReg //PIND

/************************************************************************/
/*                        Public Enum Typedefs                          */
/************************************************************************/

/************************************************************************/
/*                           APIs PROTOTYPES                            */
/************************************************************************/
/**
 * Function : sw_uart_init
 * Description: This function is to initialize the Software UART Module.
 * @return nothing 
 */
void sw_uart_init(void);

/**
 * Function : InverterCom_Update
 * Description: This function is to update the Communication Module
 * @return nothing 
 */
void sw_uart_update(void* args);

/**
 * Function : sw_uart_send
 * Description: This function is used to send a byte through the uart
 * @param data is the data to be sent
 * @return nothing 
 */
void sw_uart_send(uint8_t data);

/**
 * Function : sw_uart_read
 * Description: This function is used to read a byte through the uart
 * @return the data received
 */
uint8_t sw_uart_read(void);

/**
 * Function : sw_uart_TxComplete
 * Description: This function is used to read the status of the transmission compilation
 * @return the status of the transmission compilation [TRUE, or FALSE]
 */
bool_t sw_uart_TxComplete(void);

/**
 * Function : sw_uart_RxComplete
 * Description: This function is used to read the status of the reception compilation
 * @return the status of the reception compilation [TRUE, or FALSE]
 */
bool_t sw_uart_RxComplete(void);

/**
 * Function : sw_uart_TxFlagClr
 * Description: This function is used to clear the TxComplete flag
 * @return Nothing
 */
void sw_uart_TxFlagClr(void);

/**
 * Function : sw_uart_RxFlagClr
 * Description: This function is used to clear the RxComplete flag
 * @return Nothing
 */
void sw_uart_RxFlagClr(void);

/**
 * Function : ISR_EXTERNAL
 * Description: This ISR is an External Interrupt that will fire when Falling edge occurs on the Rx Pin
 * @return nothing
 */
void sw_uart_isr_handler(void);

#endif /* _SW_UART_H__ */