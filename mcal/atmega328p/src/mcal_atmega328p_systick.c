#include "mcal.h"
#include "utils.h"

static uint8_t gu8_noOfOverflows, gu8_timerLoader;
static uint8_t gu8_noOfOverflows_counter;
static volatile uint8_t gu8_systick_flag;

void system_run(void);

void mcal_sysTick_init(void)
{
    gu8_noOfOverflows = 0;
    gu8_timerLoader = 0;
    gu8_noOfOverflows_counter = 0;
    gu8_systick_flag = 0;
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
    gx_timer.timerState = MCAL_TIMER_STOP;

    if (u32_tickms == 1)
    {
        /* SET THE PRESCALLER TO 64 AS IT GIVES THE MOST ACCURACY */
        gx_timer.timer_prescaller = PRESCALLER_64;

        /* set the interrupt enable for the compare on the register A */
        gx_timer.timer_intMode = MCAL_TIMER_INT_COMPARE_MATCH_A;

        /* get the load value of the compare match */
        u32_temp = (u32_tickms * F_CPU / (64UL * 1000UL)) - 1;

        /* set the value to the ocr register */
        OCR0A = (uint8_t)u32_temp;

        /* set the timer mode to ctc mode */
        BIT_CLR(TCCR0A, 0);
        BIT_SET(TCCR0A, 1);
        BIT_CLR(TCCR0B, 3);
    }
    else
    {
        gx_timer.timer_prescaller = PRESCALLER_1024;

        /* set the interrupt enable for the overflow */
        gx_timer.timer_intMode = MCAL_TIMER_INT_OVERFLOW;

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

// mcal_systick_overflow_int()
// {
//     gu8_noOfOverflows_counter++;
//     if (gu8_noOfOverflows_counter == gu8_noOfOverflows)
//     {
//         TCNT0 = gu8_timerLoader;
//     }
//     else if (gu8_noOfOverflows_counter > gu8_noOfOverflows)
//     {
//         system_run();

//         gu8_noOfOverflows_counter = 0;
//         if (gu8_noOfOverflows > 0)
//         {
//             TCNT0 = 0x00;
//         }
//         else
//         {
//             TCNT0 = gu8_timerLoader;
//         }
//     }
//     else
//     {
//         // Do Nothing
//     }
// }

mcal_systick_compareA_int()
{
    //BIT_TOG(PORTC, 0);
    TCNT0 = 0x00;
    gu8_systick_flag = 0xff;
}

uint8_t mcal_sysTick_flag_get(void)
{
    uint8_t ret;
    ret = gu8_systick_flag;
    gu8_systick_flag = 0;
    return ret;
}
