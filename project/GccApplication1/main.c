#include "config.h"
#include "mcal.h"
#include "app.h"
#include "heartbeat.h"
#include "gpio.h"
#include "bsp.h"
#include "mcal_atmega328p.h"

int main(void)
{
    gpio_t x_heartbeat;
    x_heartbeat.port = HEARTBEAT_LED_PORT;
    x_heartbeat.pin = HEARTBEAT_LED_PIN;
    x_heartbeat.state = HEARTBEAT_LED_INIT_STATE;
    x_heartbeat.ioState = GPIO_OUTPUT;
    heartbeat_init(&x_heartbeat);

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
        //doutputModule_update();
        heartbeat_update(NULL);
        _delay_ms(1000);
    }
}
