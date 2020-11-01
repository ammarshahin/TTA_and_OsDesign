#ifndef _BSP_TMPM370_H
#define _BSP_TMPM370_H

#include "component_id.h"
#include "gpio/gpio.h"
#include "keypad/keypad.h"

/*********************************************************************/
// heartbeat
// #define HEARTBEAT2_LED_PORT (GPIO_PORTE)
// #define HEARTBEAT2_LED_PIN (GPIO_PIN6)
// #define HEARTBEAT2_LED_INIT_STATE (GPIO_LOW)

// #define HEARTBEAT_LED_PORT (GPIO_PORTE)
// #define HEARTBEAT_LED_PIN (GPIO_PIN7)
// #define HEARTBEAT_LED_INIT_STATE (GPIO_LOW)

/*********************************************************************/
// dinput
#define KEY_SYS_OFF_PORT (GPIO_PORTK)
#define KEY_SYS_OFF_PIN (GPIO_PIN1)

#define KEY_ZCD_SW_PORT (GPIO_PORTL)
#define KEY_ZCD_SW_PIN (GPIO_PIN1)

#define LOCK_SENSOR_PORT (GPIO_PORTD)
#define LOCK_SENSOR_PIN (GPIO_PIN0)

#define MECH_SENSOR_PORT (GPIO_PORTH)
#define MECH_SENSOR_PIN (GPIO_PIN0)

/*********************************************************************/
// doutput
#define DRAIN_PUMP_PORT (GPIO_PORTG)
#define DRAIN_PUMP_PIN (GPIO_PIN0)

#define DRAIN_VALVE_PORT (GPIO_PORTD)
#define DRAIN_VALVE_PIN (GPIO_PIN2)

#define LID_LOCK_PORT (GPIO_PORTD)
#define LID_LOCK_PIN (GPIO_PIN1)

#define COLD_WATER_VALVE_PORT (GPIO_PORTD)
#define COLD_WATER_VALVE_PIN (GPIO_PIN4)

#define HOT_WATER_VALVE_PORT (GPIO_PORTG)
#define HOT_WATER_VALVE_PIN (GPIO_PIN1)

#define SOFTENER_WATER_VALVE_PORT (GPIO_PORTD)
#define SOFTENER_WATER_VALVE_PIN (GPIO_PIN3)

#define RELAY_PORT (GPIO_PORTE)
#define RELAY_PIN (GPIO_PIN7)

#define NIDEC_RELAY_PORT (GPIO_PORTC)
#define NIDEC_RELAY_PIN (GPIO_PIN2)

#define SPRAY_VALVE_PORT (GPIO_PORTG)
#define SPRAY_VALVE_PIN (GPIO_PIN2)

/*********************************************************************/
// analog inputs
#define AC_VOLTAGE_BLOCK
#define AC_VOLTAGE_CH

#define TEMP_SENSOR_WATER_SENSOR_ADC_BLK (ADC_BLK_A)
#define TEMP_SENSOR_WATER_SENSOR_ADC_CH (ADC_CH_2)
#define TEMP_SENSOR_WATER_SENSOR_ADC_PORT (GPIO_PORTH)
#define TEMP_SENSOR_WATER_SENSOR_ADC_PIN (GPIO_PIN2)

#define TEMP_SENSOR_PCB_SENSOR_ADC_BLK
#define TEMP_SENSOR_PCB_SENSOR_ADC_CH
#define TEMP_SENSOR_PCB_SENSOR_ADC_PORT
#define TEMP_SENSOR_PCB_SENSOR_ADC_PIN

#define LID_SENSOR_1_ADC_BLK (ADC_BLK_A)
#define LID_SENSOR_1_ADC_CH (ADC_CH_1)
#define LID_SENSOR_1_ADC_PORT (GPIO_PORTH)
#define LID_SENSOR_1_ADC_PIN (GPIO_PIN1)

/*********************************************************************/
// freq inputs
#define WATER_LEVEL_PORT (GPIO_PORTA)
#define WATER_LEVEL_PIN (GPIO_PIN7)

#define DRAIN_VALVE_SENSOR_PORT (GPIO_PORTE)
#define DRAIN_VALVE_SENSOR_PIN (GPIO_PIN6)

/*********************************************************************/
// freq outputs
#define BUZZER_PORT GPIO_PORTE
#define BUZZER_PIN GPIO_PIN5

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
#define KEYPAD_COL_0_PORT (GPIO_PORTG)
#define KEYPAD_COL_0_PIN (GPIO_PIN5)
#define KEYPAD_COL_1_PORT (GPIO_PORTG)
#define KEYPAD_COL_1_PIN (GPIO_PIN6)
#define KEYPAD_COL_2_PORT (GPIO_PORTJ)
#define KEYPAD_COL_2_PIN (GPIO_PIN7)
#define KEYPAD_COL_3_PORT (GPIO_PORTB)
#define KEYPAD_COL_3_PIN (GPIO_PIN1)

#define KEYPAD_ROW_0_PORT (GPIO_PORTG)
#define KEYPAD_ROW_0_PIN (GPIO_PIN4)
#define KEYPAD_ROW_1_PORT (GPIO_PORTG)
#define KEYPAD_ROW_1_PIN (GPIO_PIN3)
#define KEYPAD_ROW_2_PORT (GPIO_PORTC)
#define KEYPAD_ROW_2_PIN (GPIO_PIN0)
#define KEYPAD_ROW_3_PORT
#define KEYPAD_ROW_3_PIN

/*********************************************************************/
// led matrix
#define LED_MATRIX_COL_0_PORT (GPIO_PORTG)
#define LED_MATRIX_COL_0_PIN (GPIO_PIN5)
#define LED_MATRIX_COL_1_PORT (GPIO_PORTG)
#define LED_MATRIX_COL_1_PIN (GPIO_PIN6)
#define LED_MATRIX_COL_2_PORT (GPIO_PORTJ)
#define LED_MATRIX_COL_2_PIN (GPIO_PIN7)
#define LED_MATRIX_COL_3_PORT (GPIO_PORTB)
#define LED_MATRIX_COL_3_PIN (GPIO_PIN1)

#define LED_MATRIX_ROW_0_PORT (GPIO_PORTF)
#define LED_MATRIX_ROW_0_PIN (GPIO_PIN3)
#define LED_MATRIX_ROW_1_PORT (GPIO_PORTF)
#define LED_MATRIX_ROW_1_PIN (GPIO_PIN2)
#define LED_MATRIX_ROW_2_PORT (GPIO_PORTF)
#define LED_MATRIX_ROW_2_PIN (GPIO_PIN1)
#define LED_MATRIX_ROW_3_PORT (GPIO_PORTG)
#define LED_MATRIX_ROW_3_PIN (GPIO_PIN7)
#define LED_MATRIX_ROW_4_PORT (GPIO_PORTB)
#define LED_MATRIX_ROW_4_PIN (GPIO_PIN0)
#define LED_MATRIX_ROW_5_PORT (GPIO_PORTJ)
#define LED_MATRIX_ROW_5_PIN (GPIO_PIN5)
#define LED_MATRIX_ROW_6_PORT (GPIO_PORTB)
#define LED_MATRIX_ROW_6_PIN (GPIO_PIN2)
#define LED_MATRIX_ROW_7_PORT (GPIO_PORTK)
#define LED_MATRIX_ROW_7_PIN (GPIO_PIN0)

/*********************************************************************/
// ssd
#define SSD_DIGIT_1_PORT (GPIO_PORTG)
#define SSD_DIGIT_1_PIN (GPIO_PIN5)
#define SSD_DIGIT_0_PORT (GPIO_PORTG)
#define SSD_DIGIT_0_PIN (GPIO_PIN6)
#define SSD_DIGIT_3_PORT (GPIO_PORTJ)
#define SSD_DIGIT_3_PIN (GPIO_PIN7)
#define SSD_DIGIT_2_PORT (GPIO_PORTB)
#define SSD_DIGIT_2_PIN (GPIO_PIN1)

#define SSD_DATA_A_PORT (GPIO_PORTE)
#define SSD_DATA_A_PIN (GPIO_PIN3)
#define SSD_DATA_B_PORT (GPIO_PORTE)
#define SSD_DATA_B_PIN (GPIO_PIN4)
#define SSD_DATA_C_PORT (GPIO_PORTA)
#define SSD_DATA_C_PIN (GPIO_PIN6)
#define SSD_DATA_D_PORT (GPIO_PORTH)
#define SSD_DATA_D_PIN (GPIO_PIN4)
#define SSD_DATA_E_PORT (GPIO_PORTA)
#define SSD_DATA_E_PIN (GPIO_PIN4)
#define SSD_DATA_F_PORT (GPIO_PORTA)
#define SSD_DATA_F_PIN (GPIO_PIN2)
#define SSD_DATA_G_PORT (GPIO_PORTE)
#define SSD_DATA_G_PIN (GPIO_PIN2)
#define SSD_DATA_DP_PORT (GPIO_PORTA)
#define SSD_DATA_DP_PIN (GPIO_PIN5)

#endif
