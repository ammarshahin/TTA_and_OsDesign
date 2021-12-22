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
#include "os.h"
#include "config.h"
#include "utils.h"

/* Scheduler Data structure */
typedef struct
{
    uint16_t delay_ms;
    uint16_t periodicity_ms;
    p_fn_t function;
    void *args;
} os_task_t;

static volatile os_task_t gArr_TaskHandler[OS_SCHEDULER_TOTAL_TASK_NUMBER];
static volatile uint8_t gu8_internalTaskCounter;

static void os_scheduler_runTasks(void);

/* Scheduler Init function */
void os_scheduler_init(void)
{
    uint8_t u8_index;
    for (u8_index = 0; u8_index < OS_SCHEDULER_TOTAL_TASK_NUMBER; u8_index++)
    {
        gArr_TaskHandler[u8_index].delay_ms = 0;
        gArr_TaskHandler[u8_index].periodicity_ms = 0;
        gArr_TaskHandler[u8_index].function = NULL;
        gArr_TaskHandler[u8_index].args = NULL;
    }
    gu8_internalTaskCounter = 0;

    mcal_sysTick_init();
    mcal_sysTick_set(OS_TICK_PERIOD_MS);
}

/* Scheduler Dispatcher */
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

/* Scheduler Task create fuction */

/**
 * @function: os_scheduler_task_add 
 * @brief this function is to create Task and add it to the Operating System
 * 
 * @param function A pointer to the task function 
 * @param args A generic pointer that is used to pass parameter to the task every time it's called
 * @param delay_ms A delay that will happen to the task before it's start execution
 * @param periodicity_ms The rate of the task execution [0 for One shoot task]
 * @return int8_t The state of the function {0 for success and -1 for fail}
 */
int8_t os_scheduler_task_add(p_fn_t function, void *args, uint16_t delay_ms, uint16_t periodicity_ms)
{
    int8_t s8_error_state = -1;
    if ((function != NULL) && (gu8_internalTaskCounter < OS_SCHEDULER_TOTAL_TASK_NUMBER))
    {
        gArr_TaskHandler[gu8_internalTaskCounter].function = function;
        gArr_TaskHandler[gu8_internalTaskCounter].periodicity_ms = periodicity_ms;
        gArr_TaskHandler[gu8_internalTaskCounter].args = args;
        gArr_TaskHandler[gu8_internalTaskCounter].delay_ms = delay_ms;
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

/* Scheduler run task */
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
                os_scheduler_task_remove(gArr_TaskHandler[u8_index].function);
            }
            else
            {
                // Error!!
            }
        }
        else
        {
            if (gArr_TaskHandler[u8_index].function != NULL)
            {
                gArr_TaskHandler[u8_index].function(gArr_TaskHandler[u8_index].args);
                gArr_TaskHandler[u8_index].delay_ms = gArr_TaskHandler[u8_index].periodicity_ms - 1;
            }
            else
            {
                // Error!!
            }
        }
    }
}

/* Scheduler Deinit a task (remove a task) */
void os_scheduler_task_remove(p_fn_t task_function)
{
    uint8_t u8_index;
    bool_t b_found_flag = false;
    // Shift the rest of the tasks after word to the this task place
    for (u8_index = 0; u8_index < (gu8_internalTaskCounter - 1); u8_index++)
    {
        if (gArr_TaskHandler[u8_index].function == task_function)
        {
            b_found_flag = true;
        }

        if (b_found_flag == true)
        {
            gArr_TaskHandler[u8_index].delay_ms = gArr_TaskHandler[u8_index + 1].delay_ms;
            gArr_TaskHandler[u8_index].periodicity_ms = gArr_TaskHandler[u8_index + 1].periodicity_ms;
            gArr_TaskHandler[u8_index].function = gArr_TaskHandler[u8_index + 1].function;
            gArr_TaskHandler[u8_index].args = gArr_TaskHandler[u8_index + 1].args;
        }
    }
    /* reinitialize the first empty spot */
    gu8_internalTaskCounter--;
    gArr_TaskHandler[gu8_internalTaskCounter].delay_ms = 0;
    gArr_TaskHandler[gu8_internalTaskCounter].periodicity_ms = 0;
    gArr_TaskHandler[gu8_internalTaskCounter].function = NULL;
    gArr_TaskHandler[gu8_internalTaskCounter].args = NULL;
}