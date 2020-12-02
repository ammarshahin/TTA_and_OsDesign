/**
 * @file os.c
 * @author Ammar Shahin (ammar0shahin@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "mcal.h"
#include "config.h"
#include "utils.h"

typedef struct
{
    uint32_t delay_ms;
    uint32_t periodicity_ms;
    uint32_t internal_tick;
    p_fn_t function;
    void *args;
    uint8_t id;
} os_task_t;

static volatile os_task_t gArr_TaskHandler[OS_SCHEDULER_TOTAL_TASK_NUMBER];
static volatile uint8_t gu8_internalTaskCounter;
static void os_scheduler_runTasks(void);

void os_scheduler_init(void)
{
    uint8_t u8_index;
    gu8_internalTaskCounter = 0;
    for (u8_index = 0; u8_index < OS_SCHEDULER_TOTAL_TASK_NUMBER; u8_index++)
    {
        gArr_TaskHandler[u8_index].function = NULL;
        gArr_TaskHandler[u8_index].args = NULL;
        gArr_TaskHandler[u8_index].delay_ms = 0;
        gArr_TaskHandler[u8_index].internal_tick = 0;
        gArr_TaskHandler[u8_index].id = 0;
        gArr_TaskHandler[u8_index].periodicity_ms = 0;
    }

    mcal_sysTick_init();
    mcal_sysTick_set(OS_TICK_PERIOD_MS);
}

void os_scheduler_start(void)
{
    mcal_sysTick_start();
    while (1)
    {
        if (mcal_sysTick_flag_get())
        {
            mcal_sysTick_flag_clear();
            os_scheduler_runTasks();
        }
        WDT_FEED();
    }
}

/**
 * @function: os_scheduler_task_add 
 * @brief this function is to create Task and add it to the Operating System
 * 
 * @param function A pointer to the task function 
 * @param args A generic pointer that is used to pass parameter to the task every time it's called
 * @param delay_ms A delay that will happen to the task before it's start execution
 * @param periodicity_ms The rate of the task execution [0 for One shoot task]
 * @return int8_t The state of the function {0 for success and -1 for failour}
 */
int8_t os_scheduler_task_add(p_fn_t function, void *args, uint32_t delay_ms, uint32_t periodicity_ms)
{
    int8_t s8_error_state = -1;
    if ((function != NULL) || (gu8_internalTaskCounter >= OS_SCHEDULER_TOTAL_TASK_NUMBER))
    {
        gArr_TaskHandler[gu8_internalTaskCounter].function = function;
        gArr_TaskHandler[gu8_internalTaskCounter].periodicity_ms = periodicity_ms;
        gArr_TaskHandler[gu8_internalTaskCounter].args = args;
        gArr_TaskHandler[gu8_internalTaskCounter].delay_ms = delay_ms;
        gArr_TaskHandler[gu8_internalTaskCounter].id = gu8_internalTaskCounter;
        gArr_TaskHandler[gu8_internalTaskCounter].internal_tick = 0;
        gu8_internalTaskCounter++;
        s8_error_state = 0;
    }
    else
    {
        // error
        s8_error_state = -1;
    }
    return s8_error_state;
}

static void os_scheduler_runTasks(void)
{
    uint8_t u8_index;
    for (u8_index = 0; u8_index < gu8_internalTaskCounter; u8_index++)
    {
        if (gArr_TaskHandler[u8_index].delay_ms > 0)   // delay task
        {
            gArr_TaskHandler[u8_index].delay_ms--;
        }
        else if (gArr_TaskHandler[u8_index].periodicity_ms == 0)   // One Shoot task
        {
            if (gArr_TaskHandler[u8_index].function != NULL)
            {
                gArr_TaskHandler[u8_index].function(gArr_TaskHandler[u8_index].args);
                // FIXME: create this function and move all the tasks
                //os_scheduler_task_deinit(gArr_TaskHandler[u8_index].id)
                // TODO: remove this line
                gArr_TaskHandler[u8_index].function = NULL;
            }
        }
        else
        {
            gArr_TaskHandler[u8_index].internal_tick++;
            if (gArr_TaskHandler[u8_index].internal_tick == gArr_TaskHandler[u8_index].periodicity_ms)
            {
                gArr_TaskHandler[u8_index].internal_tick = 0;
                if (gArr_TaskHandler[u8_index].function != NULL)
                {
                    gArr_TaskHandler[u8_index].function(gArr_TaskHandler[u8_index].args);
                }
                else
                {
                    // Error!!
                }
            }
        }
    }
}