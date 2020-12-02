#include "mcal.h"
#include "app.h"
#include "bsp.h"
#include "mcal.h"
#include "utils.h"
#include "os.h"

int main(void)
{
    // bsp_clockConfig_set();
    // bsp_interruptPriorities_set();

    // os_runTime_init();

    os_scheduler_init();
    // hwTimer_init();
    app_init();

#if APP_USE_WDT == 1
    mcal_wdt_init();
    //wdt_init();
    // os_scheduler_task_add(wdt_update, NULL, 0, WDT_UPDATE_PERIOD_MS);
#endif

    // os_scheduler_task_add(os_runTime_update, NULL, APP_RUN_TIME_TASK_DELAY, APP_RUN_TIME_TASK_UPDATE_PERIOD_MS);
    globalInterrupts_On();
    os_scheduler_start();

    while (1)
    {
        // The code shouldn't reach this point never
    }
    return 0;
}
