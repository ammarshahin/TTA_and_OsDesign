#ifndef BSP_ATMEGA328P_H
#define BSP_ATMEGA328P_H

#include "mcal_atmega328p.h"
#include "component_id.h"
#include "gpio.h"

/*********************************************************************/
// heartbeat
#define HEARTBEAT_LED_PORT (GPIO_PORTC)
#define HEARTBEAT_LED_PIN (GPIO_PIN0)
#define HEARTBEAT_LED_INIT_STATE (GPIO_LOW)

/*********************************************************************/
// dinput

/*********************************************************************/
// doutput

/*********************************************************************/
// analog inputs
#define AC_VOLTAGE_BLOCK
#define AC_VOLTAGE_CH

/*********************************************************************/
// freq inputs

/*********************************************************************/
// freq outputs

/*********************************************************************/
// stwi
#define TWI_DATA_PORT (GPIO_PORTA)
#define TWI_DATA_PIN (GPIO_PIN3)
#define TWI_CLOCK_PORT (GPIO_PORTA)
#define TWI_CLOCK_PIN (GPIO_PIN1)

/*********************************************************************/
// uart
#define UART_0_TX_PORT (GPIO_PORTE)
#define UART_0_TX_PIN (GPIO_PIN0)
#define UART_0_RX_PORT (GPIO_PORTE)
#define UART_0_RX_PIN (GPIO_PIN1)

#define UART_1_TX_PORT (GPIO_PORTA)
#define UART_1_TX_PIN (GPIO_PIN5)
#define UART_1_RX_PORT (GPIO_PORTA)
#define UART_1_RX_PIN (GPIO_PIN6)

#define UART_2_TX_PORT (GPIO_PORTD)
#define UART_2_TX_PIN (GPIO_PIN5)
#define UART_2_RX_PORT (GPIO_PORTD)
#define UART_2_RX_PIN (GPIO_PIN6)

#define UART_3_TX_PORT (GPIO_PORTF)
#define UART_3_TX_PIN (GPIO_PIN3)
#define UART_3_RX_PORT (GPIO_PORTF)
#define UART_3_RX_PIN (GPIO_PIN4)

/*********************************************************************/
// keypad pins

/*********************************************************************/
// led matrix

/*********************************************************************/
// ssd

#endif
