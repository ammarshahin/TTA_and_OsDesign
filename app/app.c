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
#include "dinput_module.h"
#include "utils.h"

static void sysHealth_update(void *arg);
static void app_tasks_create(void);

void app_init(void)
{
    app_startup_init();
    app_startupTasks_create();

#if APP_SYS_HEALTH == 1
    os_scheduler_task_add(sysHealth_update, NULL, 500, 500);
#endif

#if APP_SYS_DEBUG == 1
    debug_init();
    os_scheduler_task_add(debug_update, NULL, 0, 1);
#endif

    app_tasks_create();
    //LOG_I("App Init Done..!");
}

static void sysHealth_update(void *arg)
{
    //
}

static void app_tasks_create(void)
{
    //
}