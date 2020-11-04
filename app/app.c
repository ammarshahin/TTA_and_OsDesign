#include "app_startup.h"

void app_init(void)
{
    app_startup_init();
    //app_startupTasks_create();

#if APP_SYS_HEALTH == 1
    os_scheduler_task_add(sysHealth_update, NULL, 500, 500);
#endif

    //LOG_I("App Init Done..!");
}

static void app_mode(void)
{
    //
}

static void sysHealth_update(void *arg)
{
    //
}
