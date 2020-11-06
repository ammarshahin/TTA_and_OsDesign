#ifndef MCAL_ATMEGA328P_H_
#define MCAL_ATMEGA328P_H_

#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include <stdint-gcc.h>
#include "inttypes.h"
#include "bsp.h"
#include "def.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "util/twi.h"
#include "avr/wdt.h"
#include "avr/common.h"

/******************************************************************************/
// gpio
#define GPIO_BIT_0 ((uint8_t)0x01)
#define GPIO_BIT_1 ((uint8_t)0x02)
#define GPIO_BIT_2 ((uint8_t)0x04)
#define GPIO_BIT_3 ((uint8_t)0x08)
#define GPIO_BIT_4 ((uint8_t)0x10)
#define GPIO_BIT_5 ((uint8_t)0x20)
#define GPIO_BIT_6 ((uint8_t)0x40)
#define GPIO_BIT_7 ((uint8_t)0x80)
#define GPIO_BIT_ALL ((uint8_t)0xFF)

#define GPIO_INPUT_MODE ((uint8_t)0x00)
#define GPIO_OUTPUT_MODE ((uint8_t)0x01)
#define GPIO_IO_MODE_NONE ((uint8_t)0x02)

#define GPIO_BIT_VALUE_1 ((uint8_t)0x01)
#define GPIO_BIT_VALUE_0 ((uint8_t)0x00)

typedef uint8_t mcal_gpio_portEnum_t;
#define MCAL_GPIO_PORTB 0u
#define MCAL_GPIO_PORTC 1u
#define MCAL_GPIO_PORTD 2u

typedef uint8_t mcal_gpio_pinEnum_t;
#define MCAL_GPIO_PIN0 GPIO_BIT_0
#define MCAL_GPIO_PIN1 GPIO_BIT_1
#define MCAL_GPIO_PIN2 GPIO_BIT_2
#define MCAL_GPIO_PIN3 GPIO_BIT_3
#define MCAL_GPIO_PIN4 GPIO_BIT_4
#define MCAL_GPIO_PIN5 GPIO_BIT_5
#define MCAL_GPIO_PIN6 GPIO_BIT_6
#define MCAL_GPIO_PIN7 GPIO_BIT_7
#define MCAL_GPIO_PIN_ALL GPIO_BIT_ALL

typedef uint8_t mcal_gpio_ioStateEnum_t;
#define MCAL_GPIO_INPUT GPIO_INPUT_MODE
#define MCAL_GPIO_OUTPUT GPIO_OUTPUT_MODE
#define MCAL_GPIO_NONE GPIO_IO_MODE_NONE

typedef uint8_t mcal_gpio_stateEnum_t;
#define MCAL_GPIO_LOW GPIO_BIT_VALUE_0
#define MCAL_GPIO_HIGH GPIO_BIT_VALUE_1

typedef enum
{
    MCAL_GPIO_NORMAL,
    MCAL_GPIO_ALT_FUNC_1,
    MCAL_GPIO_ALT_FUNC_2,
    MCAL_GPIO_ALT_FUNC_3
} mcal_gpio_alternateFnEnum_t;

typedef struct
{
    mcal_gpio_portEnum_t port;
    mcal_gpio_pinEnum_t pin;
    mcal_gpio_ioStateEnum_t ioState;
    mcal_gpio_stateEnum_t pinState;
} mcal_gpio_t;

void mcal_gpio_init(void);
void mcal_gpio_pin_init(mcal_gpio_t *px_gpio);
void mcal_gpio_pinState_set(mcal_gpio_t *px_gpio, mcal_gpio_stateEnum_t u8_state);
void mcal_gpio_portState_set(mcal_gpio_t *px_gpio, mcal_gpio_stateEnum_t x_state);
void mcal_gpio_pinIOState_set(mcal_gpio_t *px_gpio, mcal_gpio_ioStateEnum_t x_ioState);
void mcal_gpio_pinInPullupState(mcal_gpio_portEnum_t port, mcal_gpio_pinEnum_t pin, bool_t pullupEnable);
void mcal_gpio_pinState_toggle(mcal_gpio_t *px_gpio);
uint8_t mcal_gpio_pinState_get(mcal_gpio_t *px_gpio);
uint8_t mcal_gpio_portState_get(mcal_gpio_t *px_gpio);
void mcal_gpio_altFunction_set(mcal_gpio_t *px_gpio, mcal_gpio_alternateFnEnum_t x_func);

/********************************************************************************/
// uart

typedef uint8_t mcal_uart_t;
#define MCAL_UART_UART0 (0)
#define MCAL_UART_UART1 (1)
#define MCAL_UART_UART2 (2)
#define MCAL_UART_UART3 (3)

//TODO: reinit these values from the datasheet
typedef uint32_t mcal_uartDataBitsEnum_t;
#define MCAL_UART_DATA_BITS_7 (UART_DATA_BITS_7)
#define MCAL_UART_DATA_BITS_8 (UART_DATA_BITS_8)
#define MCAL_UART_DATA_BITS_9 (UART_DATA_BITS_9)

typedef uint32_t mcal_uartStopBitsEnum_t;
#define MCAL_UART_STOP_BITS_1 (UART_STOP_BITS_1)
#define MCAL_UART_STOP_BITS_2 (UART_STOP_BITS_2)

typedef uint32_t mcal_uartParityEnum_t;
#define MCAL_UART_PARITY_NO (UART_NO_PARITY)
#define MCAL_UART_PARITY_EVEN (UART_EVEN_PARITY)
#define MCAL_UART_PARITY_ODD (UART_ODD_PARITY)

typedef uint32_t mcal_uartModeEnum_t;
#define MCAL_UART_MODE_RX (UART_ENABLE_RX)
#define MCAL_UART_MODE_TX (UART_ENABLE_TX)
#define MCAL_UART_MODE_TXRX (MCAL_UART_MODE_RX | MCAL_UART_MODE_TX)

typedef uint32_t mcal_uartFlowControlEnum_t;
#define MCAL_UART_FLOW_CTRL_NONE (UART_NONE_FLOW_CTRL)

typedef uint32_t mcal_uartDirectionEnum_t;
#define MCAL_UART_DIR_RX (UART_RX)
#define MCAL_UART_DIR_TX (UART_TX)

typedef struct
{
    uint32_t baudRate;
    mcal_uartDataBitsEnum_t dataBits;
    mcal_uartStopBitsEnum_t stopBits;
    mcal_uartParityEnum_t parity;
    mcal_uartModeEnum_t mode;
    mcal_uartFlowControlEnum_t flowControl;
    uint8_t rxInterruptEN;
} mcal_uartConfig_t;

void mcal_uart_init(void);
void mcal_uart_channel_set(mcal_uart_t x_uart, mcal_uartConfig_t *px_uartConfig);
void mcal_uart_data_put(mcal_uart_t x_uart, uint8_t u8_data);
uint8_t mcal_uart_data_get(mcal_uart_t x_uart);
void mcal_uart_dma_init(void);
uint32_t mcal_uart_dma_set(uint8_t *pu8_txBuffer, uint32_t u32_length, uint32_t u32_channel);
uint32_t mcal_uart_dma_get(uint8_t *pu8_rxBuffer, uint32_t u32_length, uint32_t u32_channel);
void mcal_uart_string_put(mcal_uart_t x_uart, uint8_t *pu8_ptr);
void mcal_uart_string_get(mcal_uart_t x_uart, uint8_t *pu8_ptr);

/********************************************************************************/
// timer
/* Registers mapping */

#define TIMSK0 *((reg_type)0x6E)
#define TIMSK1 *((reg_type)0x6F)
#define TIMSK2 *((reg_type)0x70)

#define TIFR0 *((reg_type)0x35)
#define TIFR1 *((reg_type)0x36)
#define TIFR2 *((reg_type)0x37)

#define TWCR *((reg_type)0xbc)

/* Timer 0 */
#define TCCR0A *((reg_type)0x44)
#define TCCR0B *((reg_type)0x45)
#define TCNT0 *((reg_type)0x46)
#define OCR0A *((reg_type)0x47)
#define OCR0B *((reg_type)0x48)

/* Timer 2 */
#define TCCR2A *((reg_type)0xB0)
#define TCCR2B *((reg_type)0xB1)
#define TCNT2 *((reg_type)0xB2)
#define OCR2A *((reg_type)0xB3)
#define OCR2B *((reg_type)0xB4)

/* Timer 1 */
#define TCCR1A *((reg_type)0x80)
#define TCCR1B *((reg_type)0x81)
#define TCCR1C *((reg_type)0x82)
#define TCNT1L *((reg_type)0x84)
#define TCNT1H *((reg_type)0x85)
#define ICR1L *((reg_type)0x86)
#define ICR1H *((reg_type)0x87)
#define OCR1AL *((reg_type)0x88)
#define OCR1AH *((reg_type)0x89)
#define OCR1BL *((reg_type)0x8A)
#define OCR1BH *((reg_type)0x8B)

typedef enum
{
    MCAL_TIMER_0 = (0U),
    MCAL_TIMER_1 = (1U),
    MCAL_TIMER_2 = (2U),
} mcal_timer_t;

typedef enum
{
    MCAL_TIMER_TIMER_MODE = (0U),
    MCAL_TIMER_COUNTER_MODE = (1U),
} mcal_timer_mode_t;

typedef enum
{
    MCAL_TIMER_STOP = (0U),
    MCAL_TIMER_START = (1U),
} mcal_timer_timerState_t;

typedef enum
{
    MCAL_TIMER_EDGE_RISING = (1U),
    MCAL_TIMER_EDGE_FALLING = (0U),
} mcal_timer_eventEdgeConfig_t;

typedef enum
{
    MCAL_TIMER_INT_DISABLE = (0x00u),
    MCAL_TIMER_INT_OVERFLOW = (0x01u),
    MCAL_TIMER_INT_COMPARE_MATCH_A = (0x02u),
    MCAL_TIMER_INT_COMPARE_MATCH_B = (0x04u),
} mcal_timer_intModeEnum_t;

typedef uint8_t mcal_timer_intMode_t;

typedef enum
{
    MCAL_TIMER_INT_NO_FLAG = (0x00u),
    MCAL_TIMER_INT_OVERFLOW_FLAG = (0x01u),
    MCAL_TIMER_INT_COMPARE_MATCH_A_FLAG = (0x02u),
    MCAL_TIMER_INT_COMPARE_MATCH_B_FLAG = (0x04u),
} mcal_timer_intFlagEnum_t;

typedef enum
{
    NO_CLOCK = (0u),
    NO_PRESCALLER = (1u),
    PRESCALLER_8 = (2u),
    PRESCALLER_64 = (3u),
    PRESCALLER_256 = (4u),
    PRESCALLER_1024 = (5u),
    PRESCALLER_EXTERNAL_CLOCK_FALLING = (6u),
    PRESCALLER_EXTERNAL_CLOCK_RISING = (7u),
} mcal_timer_prescaller_t;

typedef struct
{
    mcal_timer_t channel_num;
    mcal_timer_prescaller_t timer_prescaller;
    mcal_timer_timerState_t timerState;
    mcal_timer_intMode_t timer_intMode;
} mcal_timer_CFG_t;

void mcal_timer_init(mcal_timer_CFG_t *px_timerCFG);
void mcal_timer_timerInterruptState_set(mcal_timer_t px_tb, mcal_timer_intMode_t x_intState);
void mcal_timer_timerChannel_enable(mcal_timer_t px_tb);
void mcal_timer_timerChannel_disable(mcal_timer_t px_tb);
void mcal_timer_timerCounter_reset(mcal_timer_t px_tb);
uint16_t mcal_timer_timerCounter_get(mcal_timer_t px_tb);
void mcal_timer_timerFlag_clear(mcal_timer_t px_tb, mcal_timer_intFlagEnum_t x_flag);
uint8_t mcal_timer_timerFlag_get(mcal_timer_t px_tb, mcal_timer_intFlagEnum_t x_flag);


/* stopped here */
/*
void mcal_timer_timerModeMS_init(mcal_timer_t *px_tb, uint32_t u32_timeMS);
void mcal_timer_eventMode_init(mcal_timer_t *px_tb, mcal_gpio_t *px_portConfig, mcal_timer_eventEdgeConfig_t x_edge);
void mcal_timer_softWareCap(mcal_timer_t *px_tb);
*/
/********************************************************************************/
// pwm
// TODO: redo these values
typedef struct
{
    mcal_gpio_portEnum_t port;
    mcal_gpio_pinEnum_t pin;
    uint32_t freq;
    uint32_t duty;
    uint8_t type;
} mcal_pwmConfig_t;

typedef uint8_t mcal_pwm_t;
#define MCAL_PWM_0
#define MCAL_PWM_1
#define MCAL_PWM_2
#define MCAL_PWM_3
#define MCAL_PWM_4
#define MCAL_PWM_5
#define MCAL_PWM_6
#define MCAL_PWM_7

typedef uint32_t mcal_pwm_state_t;
#define MCAL_PWM_START
#define MCAL_PWM_STOP

void mcal_pwm_init(void);
void mcal_pwm_channel_set(mcal_pwm_t *x_pwmInerface, mcal_pwmConfig_t *x_pwmConfig);
void mcal_pwm_channelState_set(mcal_pwm_t *x_pwmInerface, mcal_pwm_state_t x_state);
void mcal_pwm_channel_enable(mcal_pwm_t *x_pwmInerface);
void mcal_pwm_channel_disable(mcal_pwm_t *x_pwmInerface);

/********************************************************************************/
// adc
typedef enum
{
    MCAL_ADC_CH0 = 0U,
    MCAL_ADC_CH1 = 1U,
    MCAL_ADC_CH2 = 2U,
    MCAL_ADC_CH3 = 3U,
    MCAL_ADC_CH4 = 4U,
    MCAL_ADC_CH5 = 5U,
    MCAL_ADC_CH6,
    MCAL_ADC_CH7,
    MCAL_ADC_CH8,
    MCAL_ADC_CH9,
    MCAL_ADC_CH10,
    MCAL_ADC_CH11,
    MCAL_ADC_CH12,
    MCAL_ADC_CH13,
    MCAL_ADC_CH14,
    MCAL_ADC_CH15,
    MCAL_ADC_CH16,
    MCAL_ADC_CH17,
    MCAL_ADC_CH18,
    MCAL_ADC_CH19,
    MCAL_ADC_CH20
} mcal_adc_chEnum_t;

// TODO: redo these values
typedef uint8_t mcal_adc_blkEnum_t;
#define MCAL_ADC_BLK_A
#define MCAL_ADC_BLK_B

void mcal_adc_init(void);
void mcal_adc_set(mcal_adc_blkEnum_t *x_adcBlock);
void mcal_adc_channel_set(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);
void mcal_adc_conversion_start(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);
uint32_t mcal_adc_conversionResult_get(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);
uint32_t mcal_adc_conversionStatus_get(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);
void mcal_adc_flag_clear(mcal_adc_blkEnum_t *x_adcBlock, mcal_adc_chEnum_t x_adcCh);

#endif
