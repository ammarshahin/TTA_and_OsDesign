/**
 * @file mcal_atmega328p_timer.c
 * @author Ammar Shahin (ammar0shahin@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "mcal_atmega328p.h"
#include "def.h"
#include "utils.h"

typedef struct
{
    mcal_timer_prescaller_t mask;
    uint16_t absolute_value;
} timer_prescaller_t;

static timer_prescaller_t gArr_timers_internalHandler[3u];

static void mcal_timer_perscaller_buffer(mcal_timer_t x_timer, mcal_timer_prescaller_t prescaller);

// FIXME: add the functions comments
/**
 * @fuction: mcal_timer_init 
 * @brief 
 * 
 * @param px_timerCFG 
 */
void mcal_timer_init(mcal_timer_CFG_t *px_timerCFG)
{

    switch (px_timerCFG->channel_num)
    {
    case MCAL_TIMER_0:
        TCCR0A = 0x00u;
        TIMSK0 = (0x00u | px_timerCFG->timer_intMode);
        if (px_timerCFG->timerState == MCAL_TIMER_START)
        {
            TCCR0B = (0x00u | px_timerCFG->timer_prescaller);
        }
        else
        {
            TCCR0B = 0x00u;
        }

        break;

    case MCAL_TIMER_1:
        TCCR0A = 0x00u;
        TIMSK0 = (0x00u | px_timerCFG->timer_intMode);
        if (px_timerCFG->timerState == MCAL_TIMER_START)
        {
            TCCR0B = (0x00u | px_timerCFG->timer_prescaller);
        }
        else
        {
            TCCR0B = 0x00u;
        }

        break;

    case MCAL_TIMER_2:
        TCCR2A = 0x00u;
        TIMSK2 = (0x00u | px_timerCFG->timer_intMode);
        if (px_timerCFG->timerState == MCAL_TIMER_START)
        {
            TCCR2B = (0x00u | px_timerCFG->timer_prescaller);
        }
        else
        {
            TCCR2B = 0x00u;
        }

        break;

    default:
        break;
    }

    /* mapping the prescaller to an absolute value and buffer it */
    mcal_timer_perscaller_buffer(px_timerCFG->channel_num, px_timerCFG->timer_prescaller);
}

static void mcal_timer_perscaller_buffer(mcal_timer_t x_timer, mcal_timer_prescaller_t prescaller)
{
    gArr_timers_internalHandler[x_timer].mask = prescaller;

    switch (x_timer)
    {
    case MCAL_TIMER_0:
        switch (prescaller)
        {
        case NO_CLOCK:
            gArr_timers_internalHandler[x_timer].absolute_value = 0;
            break;
        case NO_PRESCALLER:
            gArr_timers_internalHandler[x_timer].absolute_value = 1;
            break;
        case PRESCALLER_8:
            gArr_timers_internalHandler[x_timer].absolute_value = 8;
            break;
        case PRESCALLER_64:
            gArr_timers_internalHandler[x_timer].absolute_value = 64;
            break;
        case PRESCALLER_256:
            gArr_timers_internalHandler[x_timer].absolute_value = 256;
            break;
        case PRESCALLER_1024:
            gArr_timers_internalHandler[x_timer].absolute_value = 1024;
            break;
        default:
            break;
        }

        break;

    case MCAL_TIMER_1:
        switch (prescaller)
        {
        case NO_CLOCK:
            gArr_timers_internalHandler[x_timer].absolute_value = 0;
            break;
        case NO_PRESCALLER:
            gArr_timers_internalHandler[x_timer].absolute_value = 1;
            break;
        case PRESCALLER_8:
            gArr_timers_internalHandler[x_timer].absolute_value = 8;
            break;
        case PRESCALLER_64:
            gArr_timers_internalHandler[x_timer].absolute_value = 64;
            break;
        case PRESCALLER_256:
            gArr_timers_internalHandler[x_timer].absolute_value = 256;
            break;
        case PRESCALLER_1024:
            gArr_timers_internalHandler[x_timer].absolute_value = 1024;
            break;
        default:
            break;
        }
        break;

    case MCAL_TIMER_2:
        switch (prescaller)
        {
        case NO_CLOCK:
            gArr_timers_internalHandler[x_timer].absolute_value = 0;
            break;
        case NO_PRESCALLER:
            gArr_timers_internalHandler[x_timer].absolute_value = 1;
            break;
        case PRESCALLER_8:
            gArr_timers_internalHandler[x_timer].absolute_value = 8;
            break;
        case PRESCALLER_64:
            gArr_timers_internalHandler[x_timer].absolute_value = 32;
            break;
        case PRESCALLER_256:
            gArr_timers_internalHandler[x_timer].absolute_value = 64;
            break;
        case PRESCALLER_1024:
            gArr_timers_internalHandler[x_timer].absolute_value = 128;
            break;
        case PRESCALLER_EXTERNAL_CLOCK_FALLING:
            gArr_timers_internalHandler[x_timer].absolute_value = 256;
            break;
        case PRESCALLER_EXTERNAL_CLOCK_RISING:
            gArr_timers_internalHandler[x_timer].absolute_value = 1024;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void mcal_timer_timerInterruptState_set(mcal_timer_t px_timer, mcal_timer_intMode_t x_intState)
{
    switch (px_timer)
    {
    case MCAL_TIMER_0:
        TIMSK0 = ((TIMSK0 & 0xf8) | x_intState);
        break;
    case MCAL_TIMER_1:
        // TODO:
        break;
    case MCAL_TIMER_2:
        TIMSK0 = ((TIMSK0 & 0xf8) | x_intState);
        break;
    default:
        break;
    }
}

void mcal_timer_timerChannel_enable(mcal_timer_t px_timer)
{
    switch (px_timer)
    {
    case MCAL_TIMER_0:
        TCCR0B = ((TCCR0B & 0xf8) | gArr_timers_internalHandler[px_timer].mask);
        break;
    case MCAL_TIMER_1:
        // TODO:
        break;
    case MCAL_TIMER_2:
        TCCR2B = ((TCCR2B & 0xf8) | gArr_timers_internalHandler[px_timer].mask);
        break;
    default:
        break;
    }
}

void mcal_timer_timerChannel_disble(mcal_timer_t px_timer)
{
    switch (px_timer)
    {
    case MCAL_TIMER_0:
        TCCR0B = (TCCR0B & 0xf8);
        break;
    case MCAL_TIMER_1:
        // TODO:
        break;
    case MCAL_TIMER_2:
        TCCR2B = (TCCR2B & 0xf8);
        break;
    default:
        break;
    }
}

void mcal_timer_timerCounter_reset(mcal_timer_t px_timer)
{
    switch (px_timer)
    {
    case MCAL_TIMER_0:
        TCNT0 = 0x00;
        break;
    case MCAL_TIMER_1:
        TCNT1L = 0x00;
        TCNT1H = 0x00;
        break;
    case MCAL_TIMER_2:
        TCNT2 = 0x00;
        break;
    default:
        break;
    }
}

uint16_t mcal_timer_timerCounter_get(mcal_timer_t px_timer)
{
    uint16_t ret = 0;
    switch (px_timer)
    {
    case MCAL_TIMER_0:
        ret = (uint16_t)TCNT0;
        break;
    case MCAL_TIMER_1:
        ret = ((uint16_t)TCNT1L | ((uint16_t)TCNT1H << 8));
        break;
    case MCAL_TIMER_2:
        ret = (uint16_t)TCNT2;
        break;
    default:
        break;
    }
    return ret;
}

void mcal_timer_timerFlag_clear(mcal_timer_t px_timer, mcal_timer_intFlagEnum_t x_flag)
{
    switch (px_timer)
    {
    case MCAL_TIMER_0:
        TIFR0 = ((TIFR0 & (~x_flag)));
        break;
    case MCAL_TIMER_1:
        TIFR1 = ((TIFR1 & (~x_flag)));
        break;
    case MCAL_TIMER_2:
        TIFR2 = ((TIFR2 & (~x_flag)));
        break;
    default:
        break;
    }
}

uint8_t mcal_timer_timerFlag_get(mcal_timer_t px_timer, mcal_timer_intFlagEnum_t x_flag)
{
    uint8_t ret = 0;
    switch (px_timer)
    {
    case MCAL_TIMER_0:
        ret = TIFR0 & x_flag;
        break;
    case MCAL_TIMER_1:
        ret = TIFR1 & x_flag;
        break;
    case MCAL_TIMER_2:
        ret = TIFR2 & x_flag;
        break;
    default:
        break;
    }
    return ret;
}

/**
 * @function: mcal_timer_delay_ms
 * @description: Delay function pooling based to block the code for a specific amount of time in us with minimum number of 3 at prescaller 8 and 1MHZ F_CPU , and 262 at prescaller 1024 and 1MHZ F_CPU
 * @param x_timerCh the timer channel used
 * @param delay the time I want to delay in ms
 */
void mcal_timer_delay_ms(mcal_timer_t x_timerCh, uint16_t delay)
{
    volatile uint64_t counter = 0;
    volatile uint8_t *timer_flag_reg = NULL;

    switch (x_timerCh)
    {
    case MCAL_TIMER_0:
        counter = (uint64_t)(((uint64_t)delay * F_CPU) / ((uint64_t)TIMER0_MAX_COUNT * gArr_timers_internalHandler[x_timerCh].absolute_value * 1000UL));
        TCNT0 = 0u;
        timer_flag_reg = &TIFR0;
        break;

    case MCAL_TIMER_1:
        counter = (uint64_t)(((uint64_t)delay * F_CPU) / ((uint64_t)TIMER1_MAX_COUNT * gArr_timers_internalHandler[x_timerCh].absolute_value * 1000UL));
        TCNT1L = 0u;
        TCNT1H = 0u;
        timer_flag_reg = &TIFR1;
        break;

    case MCAL_TIMER_2:
        counter = (uint64_t)(((uint64_t)delay * F_CPU) / ((uint64_t)TIMER2_MAX_COUNT * gArr_timers_internalHandler[x_timerCh].absolute_value * 1000UL));
        TCNT2 = 0u;
        timer_flag_reg = &TIFR2;
        break;

    default:
        break;
    }

    mcal_timer_timerChannel_enable(x_timerCh);
    while (counter--)
    {
        while ((*timer_flag_reg & TIFR_TOV_FLAG_MASK) == false)
            ;
        *timer_flag_reg |= TIFR_TOV_FLAG_MASK;
    }
    mcal_timer_timerChannel_disble(x_timerCh);
}

/**
 * @function: mcal_timer_delay_us
 * @description: Delay function pooling based to block the code for a specific amount of time in us with minimum number of 255 at prescaller 1 and 1MHZ F_CPU ,and 261120 at prescaller 1024 and 1MHZ F_CPU
 * @param x_timerCh the timer channel used
 * @param delay the time I want to delay in us
 */
void mcal_timer_delay_us(mcal_timer_t x_timerCh, uint16_t delay)
{
    volatile uint64_t counter = 0;
    volatile uint8_t *timer_flag_reg = NULL;

    switch (x_timerCh)
    {
    case MCAL_TIMER_0:
        counter = (uint64_t)((uint64_t)delay * F_CPU) / ((uint64_t)TIMER0_MAX_COUNT * (uint64_t)gArr_timers_internalHandler[x_timerCh].absolute_value * 1000000UL);
        TCNT0 = 0u;
        timer_flag_reg = &(TIFR0);
        break;

    case MCAL_TIMER_1:
        counter = (uint64_t)((uint64_t)delay * F_CPU) / ((uint64_t)TIMER1_MAX_COUNT * (uint64_t)gArr_timers_internalHandler[x_timerCh].absolute_value * 1000000UL);
        TCNT1L = 0u;
        TCNT1H = 0u;
        timer_flag_reg = &(TIFR1);
        break;

    case MCAL_TIMER_2:
        counter = (uint64_t)((uint64_t)delay * F_CPU) / ((uint64_t)TIMER2_MAX_COUNT * (uint64_t)gArr_timers_internalHandler[x_timerCh].absolute_value * 1000000UL);
        TCNT2 = 0u;
        timer_flag_reg = &(TIFR2);
        break;

    default:
        break;
    }

    mcal_timer_timerChannel_enable(x_timerCh);
    while (counter--)
    {
        while ((*timer_flag_reg & TIFR_TOV_FLAG_MASK) == false)
            ;
        *timer_flag_reg |= TIFR_TOV_FLAG_MASK;
    }
    mcal_timer_timerChannel_disble(x_timerCh);
}

/**
 * @function: mcal_timer_delay_ns
 * @description: Delay function pooling based to block the code for a specific amount of time in us with minimum number of 255000 at prescaller 1 and 1MHZ F_CPU.
 * @param x_timerCh the timer channel used
 * @param delay the time I want to delay in ns
 */
void mcal_timer_delay_ns(mcal_timer_t x_timerCh, uint16_t delay)
{
    volatile uint64_t counter = 0;
    volatile uint8_t *timer_flag_reg = NULL;

    switch (x_timerCh)
    {
    case MCAL_TIMER_0:
        counter = (uint64_t)((uint64_t)delay * F_CPU) / ((uint64_t)TIMER0_MAX_COUNT * (uint64_t)gArr_timers_internalHandler[x_timerCh].absolute_value * 1000000000UL);
        TCNT0 = 0u;
        timer_flag_reg = &(TIFR0);
        break;

    case MCAL_TIMER_1:
        counter = (uint64_t)((uint64_t)delay * F_CPU) / ((uint64_t)TIMER1_MAX_COUNT * (uint64_t)gArr_timers_internalHandler[x_timerCh].absolute_value * 1000000000UL);
        TCNT1L = 0u;
        TCNT1H = 0u;
        timer_flag_reg = &(TIFR1);
        break;

    case MCAL_TIMER_2:
        counter = (uint64_t)((uint64_t)delay * F_CPU) / ((uint64_t)TIMER2_MAX_COUNT * (uint64_t)gArr_timers_internalHandler[x_timerCh].absolute_value * 1000000000UL);
        TCNT2 = 0u;
        timer_flag_reg = &(TIFR2);
        break;

    default:
        break;
    }

    mcal_timer_timerChannel_enable(x_timerCh);
    while (counter--)
    {
        while ((*timer_flag_reg & TIFR_TOV_FLAG_MASK) == false)
            ;
        *timer_flag_reg |= TIFR_TOV_FLAG_MASK;
    }
    mcal_timer_timerChannel_disble(x_timerCh);
}