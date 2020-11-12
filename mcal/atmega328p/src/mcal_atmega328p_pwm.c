/**
 * @file mcal_atmega328p_pwm.c
 * @author Ammar Shahin (ammar0shahin@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "mcal.h"
#include "utils.h"

typedef struct
{
    mcal_timer_t timer[MCAL_PWM_MAX_NUM];
    uint32_t prescaller[MCAL_PWM_MAX_NUM];
    uint8_t internal_counter;
} mcal_pwm_internalHandler_t;
static mcal_pwm_internalHandler_t gArr_pwm_internal_handler;

static mcal_timer_t gx_swPwm_timer;
static mcal_gpio_t gx_sw_gpio;

/**
 * @fuction: mcal_pwm_init
 * @brief 
 * 
 * @param px_pwmCFG 
 */
void mcal_pwm_init(mcal_pwmConfig_t *pwmCfg)
{
    mcal_gpio_t x_gpio;
    gArr_pwm_internal_handler.timer[gArr_pwm_internal_handler.internal_counter] = pwmCfg->timer;
    switch (pwmCfg->timer)
    {
    case MCAL_TIMER_0:
        /* set the pin as an output */
        x_gpio.port = MCAL_GPIO_PORTD;
        x_gpio.pin = MCAL_GPIO_PIN6;
        x_gpio.ioState = MCAL_GPIO_OUTPUT;
        x_gpio.pinState = MCAL_GPIO_LOW;
        mcal_gpio_pin_init(&x_gpio);

        /* set the timer to work in the pwm mode */
        BIT_SET(TCCR0A, 0);
        BIT_SET(TCCR0A, 1);
        BIT_SET(TCCR0B, 3);

        /* set the timer mode */
        BIT_SET(TCCR0A, 6);
        BIT_CLR(TCCR0A, 7);

        /* set the timer prescaller/FREQ */
        gArr_pwm_internal_handler.prescaller[gArr_pwm_internal_handler.internal_counter] = pwmCfg->freq;

        /* set the duty */
        OCR0A = (uint8_t)((pwmCfg->duty * TIMER0_MAX_COUNT) / 100);

        if (pwmCfg->state == MCAL_PWM_START)
        {
            mcal_pwm_channel_enable(gArr_pwm_internal_handler.internal_counter);
        }

        break;

    /* TODO: needs rework */
    case MCAL_TIMER_1:
        /* set the pin as an output */
        x_gpio.port = MCAL_GPIO_PORTD;
        x_gpio.pin = MCAL_GPIO_PIN6;
        x_gpio.ioState = MCAL_GPIO_OUTPUT;
        x_gpio.pinState = MCAL_GPIO_LOW;
        mcal_gpio_pin_init(&x_gpio);

        /* set the timer to work in the pwm mode */
        BIT_SET(TCCR0A, 0);
        BIT_SET(TCCR0A, 1);
        BIT_SET(TCCR0B, 3);

        /* set the timer mode */
        BIT_SET(TCCR0A, 6);
        BIT_CLR(TCCR0A, 7);

        /* set the timer prescaller/FREQ */
        gArr_pwm_internal_handler.prescaller[gArr_pwm_internal_handler.internal_counter] = pwmCfg->freq;

        /* set the duty */
        OCR0A = (uint8_t)((pwmCfg->duty * TIMER0_MAX_COUNT) / 100);

        if (pwmCfg->state == MCAL_PWM_START)
        {
            mcal_pwm_channel_enable(gArr_pwm_internal_handler.internal_counter);
        }

        break;

    case MCAL_TIMER_2:
        /* set the pin as an output */
        x_gpio.port = MCAL_GPIO_PORTB;
        x_gpio.pin = MCAL_GPIO_PIN3;
        x_gpio.ioState = MCAL_GPIO_OUTPUT;
        x_gpio.pinState = MCAL_GPIO_LOW;
        mcal_gpio_pin_init(&x_gpio);

        /* set the timer to work in the pwm mode */
        BIT_SET(TCCR2A, 0);
        BIT_SET(TCCR2A, 1);
        BIT_SET(TCCR2B, 3);

        /* set the timer mode */
        BIT_SET(TCCR2A, 6);
        BIT_CLR(TCCR2A, 7);

        /* set the timer prescaller/FREQ */
        gArr_pwm_internal_handler.prescaller[gArr_pwm_internal_handler.internal_counter] = pwmCfg->freq;

        /* set the duty */
        OCR2A = (uint8_t)((pwmCfg->duty * TIMER2_MAX_COUNT) / 100);

        if (pwmCfg->state == MCAL_PWM_START)
        {
            mcal_pwm_channel_enable(gArr_pwm_internal_handler.internal_counter);
        }

        break;

    default:
        break;
    }
    gArr_pwm_internal_handler.internal_counter++;
}

/**
 * @brief 
 * 
 * @param x_pwmInerface 
 * @param x_state 
 */
void mcal_pwm_channelState_set(mcal_pwm_t x_pwmInerface, mcal_pwm_state_t x_state)
{
}

/**
 * @brief 
 * 
 * @param x_pwmInerface 
 */
void mcal_pwm_channel_enable(mcal_pwm_t x_pwmInerface)
{
    switch (gArr_pwm_internal_handler.timer[x_pwmInerface])
    {
    case MCAL_TIMER_0:
        switch (gArr_pwm_internal_handler.prescaller[x_pwmInerface])
        {
        case MCAL_PWM_FREQ_1:
            BIT_SET(TCCR0B, 0);
            BIT_CLR(TCCR0B, 1);
            BIT_CLR(TCCR0B, 2);
            break;

        case MCAL_PWM_FREQ_8:
            BIT_CLR(TCCR0B, 0);
            BIT_SET(TCCR0B, 1);
            BIT_CLR(TCCR0B, 2);
            break;

        case MCAL_PWM_FREQ_64:
            BIT_SET(TCCR0B, 0);
            BIT_SET(TCCR0B, 1);
            BIT_CLR(TCCR0B, 2);
            break;

        case MCAL_PWM_FREQ_256:
            BIT_CLR(TCCR0B, 0);
            BIT_CLR(TCCR0B, 1);
            BIT_SET(TCCR0B, 2);
            break;

        case MCAL_PWM_FREQ_1024:
            BIT_SET(TCCR0B, 0);
            BIT_CLR(TCCR0B, 1);
            BIT_SET(TCCR0B, 2);
            break;
        default:
            BIT_CLR(TCCR0B, 0);
            BIT_CLR(TCCR0B, 1);
            BIT_CLR(TCCR0B, 2);
            break;
        }

        /* set the timer mode */
        BIT_SET(TCCR0A, 6);
        BIT_CLR(TCCR0A, 7);
        break;

        /* TODO: needs rework */
    case MCAL_TIMER_1:
        switch (gArr_pwm_internal_handler.prescaller[x_pwmInerface])
        {
        case MCAL_PWM_FREQ_1:
            BIT_SET(TCCR1B, 0);
            BIT_CLR(TCCR1B, 1);
            BIT_CLR(TCCR1B, 2);
            break;

        case MCAL_PWM_FREQ_8:
            BIT_CLR(TCCR1B, 0);
            BIT_SET(TCCR1B, 1);
            BIT_CLR(TCCR1B, 2);
            break;

        case MCAL_PWM_FREQ_64:
            BIT_SET(TCCR1B, 0);
            BIT_SET(TCCR1B, 1);
            BIT_CLR(TCCR1B, 2);
            break;

        case MCAL_PWM_FREQ_256:
            BIT_CLR(TCCR1B, 0);
            BIT_CLR(TCCR1B, 1);
            BIT_SET(TCCR1B, 2);
            break;

        case MCAL_PWM_FREQ_1024:
            BIT_SET(TCCR1B, 0);
            BIT_CLR(TCCR1B, 1);
            BIT_SET(TCCR1B, 2);
            break;

        default:
            BIT_CLR(TCCR1B, 0);
            BIT_CLR(TCCR1B, 1);
            BIT_CLR(TCCR1B, 2);
            break;
        }

        /* set the timer mode */
        BIT_SET(TCCR1A, 6);
        BIT_CLR(TCCR1A, 7);
        break;

    case MCAL_TIMER_2:
        switch (gArr_pwm_internal_handler.prescaller[x_pwmInerface])
        {
        case MCAL_PWM_FREQ_1:
            BIT_SET(TCCR2B, 0);
            BIT_CLR(TCCR2B, 1);
            BIT_CLR(TCCR2B, 2);
            break;

        case MCAL_PWM_FREQ_8:
            BIT_CLR(TCCR2B, 0);
            BIT_SET(TCCR2B, 1);
            BIT_CLR(TCCR2B, 2);
            break;

        case MCAL_PWM_FREQ_32:
            BIT_SET(TCCR2B, 0);
            BIT_SET(TCCR2B, 1);
            BIT_CLR(TCCR2B, 2);
            break;

        case MCAL_PWM_FREQ_64:
            BIT_CLR(TCCR2B, 0);
            BIT_CLR(TCCR2B, 1);
            BIT_SET(TCCR2B, 2);
            break;

        case MCAL_PWM_FREQ_128:
            BIT_SET(TCCR2B, 0);
            BIT_CLR(TCCR2B, 1);
            BIT_SET(TCCR2B, 2);
            break;

        case MCAL_PWM_FREQ_256:
            BIT_CLR(TCCR2B, 0);
            BIT_SET(TCCR2B, 1);
            BIT_SET(TCCR2B, 2);
            break;

        case MCAL_PWM_FREQ_1024:
            BIT_SET(TCCR2B, 0);
            BIT_SET(TCCR2B, 1);
            BIT_SET(TCCR2B, 2);
            break;

        default:
            BIT_CLR(TCCR0B, 0);
            BIT_CLR(TCCR0B, 1);
            BIT_CLR(TCCR0B, 2);
            break;
        }

        /* set the timer mode */
        BIT_SET(TCCR2A, 6);
        BIT_CLR(TCCR2A, 7);
        break;

    default:
        break;
    }
}

/**
 * @brief 
 * 
 * @param x_pwmInerface 
 */
void mcal_pwm_channel_disable(mcal_pwm_t x_pwmInerface)
{
    switch (gArr_pwm_internal_handler.timer[x_pwmInerface])
    {
    case MCAL_TIMER_0:
        BIT_CLR(TCCR0B, 0);
        BIT_CLR(TCCR0B, 1);
        BIT_CLR(TCCR0B, 2);

        break;
    default:
        break;
    }
}

/**
 * @funcion: mcal_pwm_swPwm_init
 * @brief 
 * 
 * @param x_timer 
 */
void mcal_pwm_swPwm_init(mcal_timer_t x_timer)
{
    mcal_timer_CFG_t ux_timer;

    ux_timer.channel_num = x_timer;
    ux_timer.timer_intMode = MCAL_TIMER_INT_DISABLE;
    ux_timer.timer_prescaller = NO_PRESCALLER;
    ux_timer.timerState = MCAL_TIMER_STOP;
    mcal_timer_init(&ux_timer);

    gx_sw_gpio.port = SWPWM_PORT;
    gx_sw_gpio.pin = SWPWM_PIN;
    gx_sw_gpio.ioState = MCAL_GPIO_OUTPUT;
    gx_sw_gpio.pinState = MCAL_GPIO_LOW;
    mcal_gpio_pin_init(&gx_sw_gpio);

    gx_swPwm_timer = x_timer;
}

/**
 * Timers_pwm_swPwm
 * Description: A function to generate a software PWM on a GPIO pin the freq, port, and the pin of the output Polling PWM. 
 * @param dutyCycle : the duty cycle of the PWM in percentage ex: 0 >> 100.
 * @param freq : the frequency of the PWM in Hz MAX: 1960 hz.
 * @param num0fCucles : the total number of cycles to be output >> 0 for infinity.
 */
void mcal_pwm_swPwm(uint8_t dutyCycle, uint64_t freq, uint64_t num0fCucles)
{
    uint32_t time_Total_us = (1000000UL / freq);
    uint32_t time_On_us = (uint32_t)(((double)dutyCycle / 100) * (double)time_Total_us);
    uint32_t time_Off_us = time_Total_us - time_On_us;

    if (num0fCucles == 0u)
    {
        while (TRUE)
        {
            mcal_gpio_pinState_set(&gx_sw_gpio, MCAL_GPIO_HIGH);
            mcal_timer_delay_us(gx_swPwm_timer, time_On_us);
            mcal_gpio_pinState_set(&gx_sw_gpio, MCAL_GPIO_LOW);
            mcal_timer_delay_us(gx_swPwm_timer, time_Off_us);
        }
    }
    else
    {
        while (num0fCucles--)
        {
            mcal_gpio_pinState_set(&gx_sw_gpio, MCAL_GPIO_HIGH);
            mcal_timer_delay_us(gx_swPwm_timer, time_On_us);
            mcal_gpio_pinState_set(&gx_sw_gpio, MCAL_GPIO_LOW);
            mcal_timer_delay_us(gx_swPwm_timer, time_Off_us);
        }
    }
}