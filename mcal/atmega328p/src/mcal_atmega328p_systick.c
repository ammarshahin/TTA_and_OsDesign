#include "mcal.h"
#include "utils.h"

static uint32_t gu32_internalTick_ms;
static mcal_timer_CFG_t gx_timer;

void system_run(void);

void mcal_sysTick_init(void)
{
    gu32_internalTick_ms = 0;
}

/**
 * @function: mcal_sysTick_set
 * @description: Function to set the interrupt to come every no of ms
 * @param u32_tickms the no to wait for 
 */
void mcal_sysTick_set(uint32_t u32_tickms)
{
    uint32_t u32_temp = 0;
    gx_timer.channel_num = MCAL_SYSTICK_TIMER_CHANNEL;
    gx_timer.timer_intMode = MCAL_TIMER_INT_OVERFLOW;
    gx_timer.timer_prescaller = PRESCALLER_1024;
    gx_timer.timerState = MCAL_TIMER_STOP;
    while (true)
    {
        u32_temp = 0;
        switch (gx_timer.timer_prescaller)
        {
        case PRESCALLER_1024:
            u32_temp = (u32_tickms * F_CPU / (1024UL * 1000UL));
            break;
        case PRESCALLER_256:
            u32_temp = (u32_tickms * F_CPU / (256UL * 1000UL));
            break;
        case PRESCALLER_64:
            u32_temp = (u32_tickms * F_CPU / (64UL * 1000UL));
            break;
        case PRESCALLER_8:
            u32_temp = (u32_tickms * F_CPU / (8UL * 1000UL));
            break;
        case NO_PRESCALLER:
            u32_temp = (u32_tickms * F_CPU / (1UL * 1000UL));
            break;
        default:
            break;
        }

        if (u32_temp >= TIMER0_MAX_COUNT)
        {
            if (gx_timer.timer_prescaller == NO_PRESCALLER)
            {
                // ERROR >> NOT VALID
                return;
            }
            else
            {
                gx_timer.timer_prescaller--;
            }
        }
        else
        {
            gu32_internalTick_ms = TIMER0_MAX_COUNT - u32_temp;
            TCNT0 = gu32_internalTick_ms;
            break;
        }
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
    system_run();
    TCNT0 = gu32_internalTick_ms;
}