/**
 * @file mcal_atmega328p_uart.c
 * @author Ammar Shahin (ammar0shahin@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "utils.h"
#include "mcal_atmega328p.h"

/**
 * @function: mcal_uart_init
 * 
 * @brief this fuction is used to initialize the main uart.
 * 
 * @param uartCFG 
 */
void mcal_uart_init(mcal_uartConfig_t *uartCFG)
{
    /* init all of the regestirs */
    UART_REG_INIT();

    /* baudrate */
    UART_BAUD_RATE_SET(uartCFG->baudRate);
    // uint16_t BaudRate = (F_CPU / (16 * uartCFG->baudRate)) - 1;
    // UART_BAUD_RATE_LOW_REG = (uint8_t)BaudRate;
    // UART_BAUD_RATE_HIGH_REG = (uint8_t)(BaudRate >> 8);

    /* uart mode */
    UART_CONTROL_STATUS_B_REG |= uartCFG->mode;

    /* uart usartEN config */
    UART_CONTROL_STATUS_C_REG |= uartCFG->usartEN;

    /* data bits */
    switch (uartCFG->dataBits)
    {
    case MCAL_UART_DATA_BITS_7:
        UART_CHAR_SIZE_7_BIT_SET();
        break;
    case MCAL_UART_DATA_BITS_8:
        UART_CHAR_SIZE_8_BIT_SET();
        break;
    case MCAL_UART_DATA_BITS_9:
        UART_CHAR_SIZE_9_BIT_SET();
        break;
    default:
        break;
    }

    /* uart stop bits */
    UART_CONTROL_STATUS_C_REG |= uartCFG->stopBits;

    /* uart parity */
    UART_CONTROL_STATUS_C_REG |= uartCFG->parity;

    /* uart interrupt */
    UART_CONTROL_STATUS_B_REG |= uartCFG->interruptEN;
}

/**
 * @brief 
 * 
 * @param x_uart 
 * @param u8_data 
 */
void mcal_uart_data_put(mcal_uart_t x_uart, uint8_t u8_data)
{
    UART_DATA_SET(u8_data);
}

/**
 * @function: mcal_uart_data_get
 * @brief this function is used to get the recivied value from the uart 
 * 
 * @param x_uart 
 * @return uint8_t 
 */
uint8_t mcal_uart_data_get(mcal_uart_t x_uart)
{
    uint8_t u8_data;
    UART_DATA_GET(u8_data);
    return u8_data;
}

/**
 * @brief 
 * 
 * @param x_uart 
 * @param pu8_ptr 
 */
void mcal_uart_string_put(mcal_uart_t x_uart, uint8_t *pu8_ptr)
{
    uint8_t u8_index = 0;
    while (pu8_ptr[u8_index] != '\0')
    {
        UART_DATA_SET(pu8_ptr[u8_index]);
        u8_index++;
    }
}

/**
 * @brief 
 * 
 * @param x_uart 
 * @param pu8_ptr 
 */
void mcal_uart_string_get(mcal_uart_t x_uart, uint8_t *pu8_ptr)
{
    uint8_t u8_index = 0;
    do
    {
        pu8_ptr[u8_index] = mcal_uart_data_get(x_uart);
    } while (pu8_ptr[u8_index++] != '*');
    pu8_ptr[u8_index - 1] = '\0';
}
