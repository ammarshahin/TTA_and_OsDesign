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

static mcal_timer_prescaller_t gArr_timers_internalHandler[3u];

// FIXME: add the function comments
/**
 * @fuction: mcal_timer_init 
 * @brief 
 * 
 * @param px_timerCFG 
 */
void mcal_timer_init(mcal_timer_CFG_t *px_timerCFG)
{

    switch (*px_timerCFG->channel_num)
    {
    case MCAL_TIMER_0:
        TCCR0A = 0x00u;
        TIMSK0 = (0x00u | px_timerCFG->timer_intMode);
        gArr_timers_internalHandler[MCAL_TIMER_0] = px_timerCFG->timer_prescaller;
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
        gArr_timers_internalHandler[MCAL_TIMER_0] = px_timerCFG->timer_prescaller;
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
        gArr_timers_internalHandler[MCAL_TIMER_2] = px_timerCFG->timer_prescaller;
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
}

void mcal_timer_timerInterruptState_set(mcal_timer_t px_tb, mcal_timer_intMode_t x_intState)
{
    switch (px_tb)
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

void mcal_timer_timerChannel_enable(mcal_timer_t px_tb)
{
    switch (px_tb)
    {
    case MCAL_TIMER_0:
        TCCR0B = ((TCCR0B & 0xf8) | gArr_timers_internalHandler[MCAL_TIMER_0]);
        break;
    case MCAL_TIMER_1:
        // TODO:
        break;
    case MCAL_TIMER_2:
        TCCR2B = ((TCCR2B & 0xf8) | gArr_timers_internalHandler[MCAL_TIMER_2]);
        break;
    default:
        break;
    }
}

void mcal_timer_timerChannel_disble(mcal_timer_t px_tb)
{
    switch (px_tb)
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

void mcal_timer_timerCounter_reset(mcal_timer_t px_tb)
{
    switch (px_tb)
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

uint16_t mcal_timer_timerCounter_get(mcal_timer_t px_tb)
{
    uint16_t ret = 0;
    switch (px_tb)
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

void mcal_timer_timerFlag_clear(mcal_timer_t px_tb, mcal_timer_intFlagEnum_t x_flag)
{
    switch (px_tb)
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

uint8_t mcal_timer_timerFlag_get(mcal_timer_t px_tb, mcal_timer_intFlagEnum_t x_flag)
{
    uint8_t ret = 0;
    switch (px_tb)
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


