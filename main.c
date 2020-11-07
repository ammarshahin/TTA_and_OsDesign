#include "config.h"
#include "mcal.h"
#include "app.h"
#include "heartbeat.h"
#include "gpio.h"
#include "bsp.h"
#include "mcal_atmega328p.h"
#include "doutput_module.h"
#include "component/dinput_module/dinput_module.h"

void doutput_module_test(void);

int main(void)
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

    mcal_timer_CFG_t x_timer;
    x_timer.channel_num = MCAL_TIMER_0;
    x_timer.timer_intMode = MCAL_TIMER_INT_DISABLE;
    x_timer.timer_prescaller = NO_PRESCALLER;
    x_timer.timerState = MCAL_TIMER_STOP;
    mcal_timer_init(&x_timer);

    // gpio_t x_heartbeat;
    // x_heartbeat.port = HEARTBEAT_LED_PORT;
    // x_heartbeat.pin = HEARTBEAT_LED_PIN;
    // x_heartbeat.state = HEARTBEAT_LED_INIT_STATE;
    // x_heartbeat.ioState = GPIO_OUTPUT;
    // heartbeat_init(&x_heartbeat);

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
        doutput_module_test();
        doutputModule_update(NULL);
        dinputModule_update(NULL);
        //heartbeat_update(NULL);
        _delay_ms(1);
    }
}

void doutput_module_test(void)
{
    static uint8_t state = MCAL_GPIO_HIGH;

    mcal_timer_delay_ms(MCAL_TIMER_0, 1000);
    //_delay_ms(1000);

    if (state == MCAL_GPIO_HIGH)
    {
        state = MCAL_GPIO_LOW;
    }
    else
    {
        state = MCAL_GPIO_HIGH;
    }

    doutputModule_state_set(HEARTBIT_OUTPUT, state);
    doutputModule_state_set(SWITCH_OUTPUT, (gpio_state_t)dinputModule_state_get(SWITCH_INPUT));
}