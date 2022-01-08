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

void app_startup_init(void)
{
    //
}

void app_startupTasks_create(void)
{
    os_scheduler_task_add(dinputModule_update, NULL, 0, DINPUT_MODULE_UPDATE_PERIOD_MS);
    os_scheduler_task_add(doutputModule_update, NULL, 0, DOUTPUT_MODULE_UPDATE_PERIOD_MS);

    // os_scheduler_task_add(dinputModule_update, NULL, 0, 1);
    // os_scheduler_task_add(app_buzzer_update, NULL, 0, APP_BUZZER_UPDATE_PERIOD);
    // os_scheduler_task_add(uart_update, NULL, 0, UART_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(adc_update, NULL, ADC_RAMPUP_DELAY_PERIOD_MS, ADC_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(doutputModule_update, NULL, 0, DOUTPUT_MODULE_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(ainputModule_update, NULL, 0, AINPUT_MODULE_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(finputModule_update, NULL, 0, FINPUT_MODULE_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(foutputModule_update, NULL, 0, FOUTPUT_MODULE_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(eeprom_update, NULL, 0, 10);
    // os_scheduler_task_add(os_timer_update, NULL, 0, OS_TIMER_UPDATE_PERIOD_MS);
    // os_scheduler_task_add(app_debug_update, NULL, 0, 500);

#if (APP_USE_HEARTBEAT == 1)
    os_scheduler_task_add(heartbeat_update, NULL, 0, HEARTBEAT_UPDATE_PERIOD_MS);
#endif
}
