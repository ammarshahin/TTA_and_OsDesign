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

typedef struct
{
    uint8_t internalBuffer[MAX_UART_BUFFER_SIZE];
    uint8_t receive_index;
    uint8_t read_index;
    uint8_t data_ready_flag;
} internal_buffer_t;

volatile static internal_buffer_t gx_internalBuffer;

/**
 * @function: mcal_uart_init
 * 
 * @brief this fuction is used to initialize the main uart.
 * 
 * @param uartCFG 
 */
void mcal_uart_init(mcal_uartConfig_t *uartCFG)
{
    uint8_t u8_index = 0;
    for (u8_index = 0; u8_index < MAX_UART_BUFFER_SIZE; u8_index++)
    {
        gx_internalBuffer.internalBuffer[u8_index] = 0;
    }
    gx_internalBuffer.read_index = 0;
    gx_internalBuffer.receive_index = 0;
    gx_internalBuffer.data_ready_flag = 0;

    /* init all of the regestirs */
    UART_REG_INIT();

    /* baudrate */
    UART_BAUD_RATE_SET(uartCFG->baudRate);

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
 * @function: mcal_uart_data_receivedFlag
 * @brief this function is used to get the recivied flag. 
 * 
 * @param x_uart the uart channel
 * @return uint8_t the data received flag
 */
uint8_t mcal_uart_data_receivedFlag_get(mcal_uart_t x_uart)
{
    return gx_internalBuffer.data_ready_flag;
}

/**
 * @function: mcal_uart_data_get
 * @brief this function is used to get the recivied value from the uart 
 * 
 * @param x_uart the uart channel
 * @return uint8_t the data received
 */
uint8_t mcal_uart_data_get(mcal_uart_t x_uart)
{
    uint8_t u8_data;

    u8_data = gx_internalBuffer.internalBuffer[gx_internalBuffer.read_index];
    gx_internalBuffer.read_index++;
    if (gx_internalBuffer.read_index >= gx_internalBuffer.receive_index)
    {
        gx_internalBuffer.read_index = 0;
        gx_internalBuffer.receive_index = 0;
        gx_internalBuffer.data_ready_flag = 0;
    }
    return u8_data;
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

/**
 * @funcion: mcal_uart_data_put 
 * @brief This function is used send data through the uart
 * 
 * @param x_uart the uart channel
 * @param u8_data the data to be sent
 */
void mcal_uart_data_put(mcal_uart_t x_uart, uint8_t u8_data)
{
    UART_DATA_SET(u8_data);
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

mcal_uart_interrupt()
{
    UART_DATA_GET(gx_internalBuffer.internalBuffer[gx_internalBuffer.receive_index]);
    gx_internalBuffer.receive_index++;
    if (gx_internalBuffer.receive_index == MAX_UART_BUFFER_SIZE)
    {
        // REPORT ERROR
        gx_internalBuffer.receive_index = 0;
        gx_internalBuffer.read_index = 0;
    }
    gx_internalBuffer.data_ready_flag = 0xff;
}