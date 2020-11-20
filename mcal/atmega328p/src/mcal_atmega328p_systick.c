#include "mcal.h"
#include "utils.h"

static uint8_t gu8_noOfOverflows, gu8_timerLoader;
static uint8_t gu8_noOfOverflows_counter;

void system_run(void);

void mcal_sysTick_init(void)
{
    gu8_noOfOverflows = 0;
    gu8_timerLoader = 0;
    gu8_noOfOverflows_counter = 0;
}

/**
 * @function: mcal_sysTick_set
 * @description: Function to set the interrupt to come every no of ms
 * @param u32_tickms the no to wait for 
 */
void mcal_sysTick_set(uint32_t u32_tickms)
{
    uint32_t u32_temp = 0;

    mcal_timer_CFG_t gx_timer;
    gx_timer.channel_num = MCAL_SYSTICK_TIMER_CHANNEL;
    gx_timer.timer_intMode = MCAL_TIMER_INT_OVERFLOW;
    gx_timer.timer_prescaller = PRESCALLER_1024;
    gx_timer.timerState = MCAL_TIMER_STOP;

    u32_temp = (u32_tickms * F_CPU / (1024UL * 1000UL));

    gu8_noOfOverflows = u32_temp / (TIMER0_MAX_COUNT + 1);
    gu8_timerLoader = u32_temp % (TIMER0_MAX_COUNT + 1);
    gu8_timerLoader = TIMER0_MAX_COUNT - gu8_timerLoader;

    if (gu8_noOfOverflows > 0)
    {
        TCNT0 = 0x00;
    }
    else
    {
        TCNT0 = gu8_timerLoader;
    }

    mcal_timer_init(&gx_timer);
}

void mcal_sysTick_start(void)
{
    mcal_timer_timerChannel_enable(MCAL_SYSTICK_TIMER_CHANNEL);
}

void mcal_sysTick_stop(void)
{
    mcal_timer_timerChannel_disable(MCAL_SYSTICK_TIMER_CHANNEL);
}

mcal_systick_int()
{
    gu8_noOfOverflows_counter++;
    if (gu8_noOfOverflows_counter == gu8_noOfOverflows)
    {
        TCNT0 = gu8_timerLoader;
    }
    else if (gu8_noOfOverflows_counter > gu8_noOfOverflows)
    {
        system_run();

        gu8_noOfOverflows_counter = 0;
        if (gu8_noOfOverflows > 0)
        {
            TCNT0 = 0x00;
        }
        else
        {
            TCNT0 = gu8_timerLoader;
        }
    }
    else
    {
        // Do Nothing
    }
}