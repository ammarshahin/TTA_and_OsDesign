#include "config.h"
#include "mcal.h"
#include "app.h"
#include "heartbeat.h"
#include "gpio.h"
#include "bsp.h"
#include "mcal.h"
#include "doutput_module.h"
#include "component/dinput_module/dinput_module.h"
#include "utils.h"

void doutput_module_test(void);
void dinput_module_test(void);
void uart_mcal_test(void);

int main(void)
{
    //sei();
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

    /*
    mcal_timer_CFG_t x_timer;
    x_timer.channel_num = MCAL_TIMER_0;
    x_timer.timer_intMode = MCAL_TIMER_INT_DISABLE;
    x_timer.timer_prescaller = NO_PRESCALLER;
    x_timer.timerState = MCAL_TIMER_STOP;
    mcal_timer_init(&x_timer);
*/
    mcal_uartConfig_t uartConfig;
    uartConfig.uart_channel = MCAL_UART_UART0;
    uartConfig.baudRate = 9600;
    uartConfig.dataBits = MCAL_UART_DATA_BITS_8;
    uartConfig.interruptEN = MCAL_UART_INTERRUPT_NONE;
    uartConfig.mode = MCAL_UART_MODE_TX | MCAL_UART_MODE_RX;
    uartConfig.parity = MCAL_UART_PARITY_NO;
    uartConfig.stopBits = MCAL_UART_STOP_BITS_1;
    uartConfig.usartEN = MCAL_UART_USART_DISABLE;
    mcal_uart_init(&uartConfig);

    mcal_pwmConfig_t pwmCfg;
    pwmCfg.timer = MCAL_TIMER_0;
    pwmCfg.duty = 20;
    pwmCfg.freq = 65;
    pwmCfg.state = MCAL_PWM_START;
    mcal_pwm_init(&pwmCfg);

    // bsp_clockConfig_set();
    // bsp_interruptPriorities_set();

    // os_runTime_init();

    // os_scheduler_init();
    // hwTimer_init();
    //app_init();

    // #if APP_USE_WDT == 1
    //     wdt_init();
    //     os_scheduler_task_add(wdt_update, NULL, 0, WDT_UPDATE_PERIOD_MS);
    // #endif

    //     os_scheduler_task_add(os_runTime_update, NULL, APP_RUN_TIME_TASK_DELAY, APP_RUN_TIME_TASK_UPDATE_PERIOD_MS);

    //     os_scheduler_start();

    while (1)
    {
        doutputModule_update(NULL);
        dinputModule_update(NULL);
        //heartbeat_update(NULL);
        doutput_module_test();
        dinput_module_test();
        //uart_mcal_test();
        _delay_ms(1);
    }
    return 0;
}

void doutput_module_test(void)
{
    static uint8_t state = MCAL_GPIO_HIGH;
    static uint16_t internalTimer = 0;
    internalTimer++;
    if (internalTimer >= 500)
    {
        internalTimer = 0;
        // mcal_timer_delay_ms(MCAL_TIMER_0, 500);
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
    else
    {
        // Do nothing
    }
}

void dinput_module_test(void)
{
    doutputModule_state_set(SWITCH_OUTPUT, dinputModule_state_get(SWITCH_INPUT));
}

void uart_mcal_test(void)
{
    static uint16_t internalTimer = 0;
    internalTimer++;
    if (internalTimer >= 500)
    {
        internalTimer = 0;
        mcal_uart_data_put(MCAL_UART_UART0, 'f');
    }
    else
    {
        //
    }
}