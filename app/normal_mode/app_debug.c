/**
 * @file debug.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-12-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "os.h"
#include "app_startup.h"
#include "app_debug.h"
#include "app.h"
#include "config.h"
#include "heartbeat.h"
#include "gpio.h"
#include "bsp.h"
#include "mcal.h"
#include "doutput_module.h"
#include "../component/dinput_module/dinput_module.h"
#include "utils.h"

mcal_pwmConfig_t pwmCfg;

void doutput_module_test(void *args);
void dinput_module_test(void *args);
void uart_mcal_test(void *args);
void pwm_test(void *args);

void debug_init(void)
{
    doutputModule_t dout;
    dout.gpio.port = HEARTBEAT_LED_PORT;
    dout.gpio.pin = HEARTBEAT_LED_PIN;
    dout.gpio.state = HEARTBEAT_LED_INIT_STATE;
    dout.gpio.ioState = GPIO_OUTPUT;
    dout.outputID = HEARTBIT_OUTPUT;
    doutputModule_init(&dout);

    dout.gpio.port = SWITCH_LED_PORT;
    dout.gpio.pin = SWITCH_LED_PIN;
    dout.gpio.state = SWITCH_LED_INIT_STATE;
    dout.gpio.ioState = GPIO_OUTPUT;
    dout.outputID = SWITCH_OUTPUT;
    doutputModule_init(&dout);

    dinputModule_t din;
    din.port = SWITCH_PORT;
    din.pin = SWITCH_PIN;
    din.pullUpEn = false;
    din.dinputID = SWITCH_INPUT;
    din.shortPressMultiplier = 50;
    din.longPressMultiplier = 1000;
    dinputModule_init(&din);

    mcal_uartConfig_t uartConfig;
    uartConfig.uart_channel = MCAL_UART_UART0;
    uartConfig.baudRate = 9600;
    uartConfig.dataBits = MCAL_UART_DATA_BITS_8;
    uartConfig.interruptEN = MCAL_UART_INTERRUPT_RX;
    uartConfig.mode = MCAL_UART_MODE_TX | MCAL_UART_MODE_RX;
    uartConfig.parity = MCAL_UART_PARITY_NO;
    uartConfig.stopBits = MCAL_UART_STOP_BITS_1;
    uartConfig.usartEN = MCAL_UART_USART_DISABLE;
    mcal_uart_init(&uartConfig);

    pwmCfg.timer = MCAL_TIMER_2;
    pwmCfg.duty = 100;
    pwmCfg.freq = 70;
    pwmCfg.state = MCAL_PWM_START;
    mcal_pwm_init(&pwmCfg);

    os_scheduler_task_add(doutput_module_test, NULL, 0, 500);
    os_scheduler_task_add(dinput_module_test, NULL, 0, 1);
    os_scheduler_task_add(pwm_test, NULL, 0, 1);
}

void debug_update(void *args)
{
    //
}

void doutput_module_test(void *args)
{
    static volatile uint8_t state = MCAL_GPIO_HIGH;
    if (state == MCAL_GPIO_HIGH)
    {
        state = MCAL_GPIO_LOW;
    }
    else
    {
        state = MCAL_GPIO_HIGH;
    }
    doutputModule_state_set(HEARTBIT_OUTPUT, state);
}

void dinput_module_test(void *args)
{
    doutputModule_state_set(SWITCH_OUTPUT, dinputModule_state_get(SWITCH_INPUT));
}

void uart_mcal_test(void *args)
{
    uint8_t temp = 0;
    static uint16_t internalTimer = 0;
    internalTimer += OS_TICK_PERIOD_MS;
    if (internalTimer == 5000)
    {
        internalTimer = 0;
        mcal_uart_data_put(MCAL_UART_UART0, 'a');
    }

    while (mcal_uart_data_receivedFlag_get(MCAL_UART_UART0))
    {
        temp = mcal_uart_data_get(MCAL_UART_UART0);
        if (temp == 1)
        {
            // doutputModule_state_set(SWITCH_OUTPUT, MCAL_GPIO_HIGH);
        }
        else
        {
            // doutputModule_state_set(SWITCH_OUTPUT, MCAL_GPIO_LOW);
        }
    }
}

void pwm_test(void *args)
{
    pwmCfg.duty -= 5;
    if (pwmCfg.duty < 5)
    {
        pwmCfg.duty = 100;
    }
    mcal_pwm_frequencyAndDuty_set(&pwmCfg);
}