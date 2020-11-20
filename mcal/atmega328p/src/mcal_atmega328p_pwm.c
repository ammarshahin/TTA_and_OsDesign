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

#define DEBUG 0

#if DEBUG
uint8_t buff[10] = {0};
#endif

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
    x_gpio.ioState = MCAL_GPIO_OUTPUT;
    x_gpio.pinState = MCAL_GPIO_LOW;

    switch (pwmCfg->timer)
    {
    case MCAL_TIMER_0:
        /* set the ocr0b pin as an output */
        x_gpio.port = MCAL_GPIO_PORTD;
        x_gpio.pin = MCAL_GPIO_PIN5;
        mcal_gpio_pin_init(&x_gpio);

        /* set the pwm mode mode{inverting/noninverting} */
        BIT_CLR(TCCR0A, COM0B0);
        BIT_SET(TCCR0A, COM0B1);

        /* set the Freq and duty */
        mcal_pwm_frequencyAndDuty_set(pwmCfg);

        if (pwmCfg->state == MCAL_PWM_START)
        {
            mcal_pwm_channel_enable(pwmCfg);
        }

        break;
#if 0
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
        BIT_CLR(TCCR0B, 3);

        /* set the timer mode */
        BIT_CLR(TCCR0A, 6);
        BIT_SET(TCCR0A, 7);

        /* set the timer prescaller/FREQ */
        prescaller = pwmCfg->freq;

        /* set the duty */
        OCR1A = (uint8_t)(((uint16_t)pwmCfg->duty * TIMER0_MAX_COUNT) / 100);

        if (pwmCfg->state == MCAL_PWM_START)
        {
            mcal_pwm_channel_enable(gArr_pwm_internal_handler.gu8_pwm_internal_counter);
        }

        break;
#endif
    case MCAL_TIMER_2:
        /* set the ocr0b pin as an output */
        x_gpio.port = MCAL_GPIO_PORTD;
        x_gpio.pin = MCAL_GPIO_PIN3;
        mcal_gpio_pin_init(&x_gpio);

        /* set the pwm mode mode{inverting/noninverting} */
        BIT_CLR(TCCR2A, COM2B0);
        BIT_SET(TCCR2A, COM2B1);

        /* set the Freq and duty */
        mcal_pwm_frequencyAndDuty_set(pwmCfg);

        if (pwmCfg->state == MCAL_PWM_START)
        {
            mcal_pwm_channel_enable(pwmCfg);
        }

        break;

    default:
        break;
    }
}

void mcal_pwm_frequencyAndDuty_set(mcal_pwmConfig_t *pwmCfg)
{
    uint16_t prescaller = 0;

    switch (pwmCfg->timer)
    {
    case MCAL_TIMER_0:

        prescaller = F_CPU / (pwmCfg->freq * (TIMER0_MAX_COUNT + 1));

        if (prescaller < 1)
        {
            prescaller = 1;
        }
        else if (prescaller < 8)
        {
            prescaller = 8;
        }
        else if (prescaller < 64)
        {
            prescaller = 64;
        }
        else if (prescaller < 256)
        {
            prescaller = 256;
        }
        else if (prescaller < 1024)
        {
            prescaller = 1024;
        }
        else
        {
            if ((pwmCfg->freq < 62) && (pwmCfg->freq > 31))
            {
                /* set the timer to work in the phase correct pwm mode */
                BIT_SET(TCCR0A, 0);
                BIT_CLR(TCCR0A, 1);
                BIT_SET(TCCR0B, 3);

                prescaller = 1024;

                OCR0A = (uint8_t)((F_CPU / (2 * pwmCfg->freq * prescaller)) - 1);

#if DEBUG
                utils_itoa(OCR0A, buff);
                mcal_uart_string_put(MCAL_UART_UART0, buff);
                mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif

                /* set the duty */
                OCR0B = (uint8_t)(((uint16_t)pwmCfg->duty * OCR0A) / 100UL);

#if DEBUG
                utils_itoa(OCR0B, buff);
                mcal_uart_string_put(MCAL_UART_UART0, buff);
                mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif
                pwmCfg->freq = (uint32_t)prescaller;
                break;
            }
            else
            {
                // Error >> Use Software PWM
                pwmCfg->freq = 0;
                break;
            }
        }

        /* set the timer to work in the fast pwm mode */
        BIT_SET(TCCR0A, 0);
        BIT_SET(TCCR0A, 1);
        BIT_SET(TCCR0B, 3);

#if DEBUG
        utils_itoa(prescaller, buff);
        mcal_uart_string_put(MCAL_UART_UART0, buff);
        mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif

        OCR0A = (uint8_t)((F_CPU / (pwmCfg->freq * prescaller)) - 1);

#if DEBUG
        utils_itoa(OCR0A, buff);
        mcal_uart_string_put(MCAL_UART_UART0, buff);
        mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif

        /* set the duty */
        OCR0B = (uint8_t)(((uint16_t)pwmCfg->duty * OCR0A) / 100UL);

#if DEBUG
        utils_itoa(OCR0B, buff);
        mcal_uart_string_put(MCAL_UART_UART0, buff);
        mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif

        pwmCfg->freq = (uint32_t)prescaller;
        break;

        // FIXME: add MCAL_TIMER_1

    case MCAL_TIMER_2:

        prescaller = F_CPU / (pwmCfg->freq * (TIMER2_MAX_COUNT + 1));

        if (prescaller < 1)
        {
            prescaller = 1;
        }
        else if (prescaller < 8)
        {
            prescaller = 8;
        }
        else if (prescaller < 32)
        {
            prescaller = 32;
        }
        else if (prescaller < 64)
        {
            prescaller = 64;
        }
        else if (prescaller < 128)
        {
            prescaller = 128;
        }
        else if (prescaller < 256)
        {
            prescaller = 256;
        }
        else if (prescaller < 1024)
        {
            prescaller = 1024;
        }
        else
        {
            if ((pwmCfg->freq < 62) && (pwmCfg->freq > 31))
            {
                /* set the timer to work in the phase correct pwm mode */
                BIT_SET(TCCR2A, 0);
                BIT_CLR(TCCR2A, 1);
                BIT_SET(TCCR2B, 3);

                prescaller = 1024;

                OCR2A = (uint8_t)((F_CPU / (2 * pwmCfg->freq * prescaller)) - 1);

#if DEBUG
                utils_itoa(OCR0A, buff);
                mcal_uart_string_put(MCAL_UART_UART0, buff);
                mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif

                /* set the duty */
                OCR2B = (uint8_t)(((uint16_t)pwmCfg->duty * OCR2A) / 100UL);

#if DEBUG
                utils_itoa(OCR0B, buff);
                mcal_uart_string_put(MCAL_UART_UART0, buff);
                mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif
                pwmCfg->freq = (uint32_t)prescaller;
                break;
            }
            else
            {
                // Error >> Use Software PWM
                pwmCfg->freq = 0;
                break;
            }
        }

        /* set the timer to work in the fast pwm mode */
        BIT_SET(TCCR2A, 0);
        BIT_SET(TCCR2A, 1);
        BIT_SET(TCCR2B, 3);

#if DEBUG
        utils_itoa(prescaller, buff);
        mcal_uart_string_put(MCAL_UART_UART0, buff);
        mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif

        OCR2A = (uint8_t)((F_CPU / (pwmCfg->freq * prescaller)) - 1);

#if DEBUG
        utils_itoa(OCR0A, buff);
        mcal_uart_string_put(MCAL_UART_UART0, buff);
        mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif

        /* set the duty */
        OCR2B = (uint8_t)(((uint16_t)pwmCfg->duty * OCR2A) / 100UL);

#if DEBUG
        utils_itoa(OCR0B, buff);
        mcal_uart_string_put(MCAL_UART_UART0, buff);
        mcal_uart_string_put(MCAL_UART_UART0, (uint8_t *)"\n");
#endif

        pwmCfg->freq = (uint32_t)prescaller;
        break;

    default:
        break;
    }
}

/**
 * @brief 
 * 
 * @param x_pwmInerface 
 * @param x_state 
 */
void mcal_pwm_channelState_set(mcal_pwmConfig_t *pwmCfg)
{
    if (pwmCfg->state == MCAL_PWM_START)
    {
        mcal_pwm_channel_enable(pwmCfg);
    }
    else if (pwmCfg->state == MCAL_PWM_STOP)
    {
        mcal_pwm_channel_disable(pwmCfg);
    }
    else
    {
        // Error
    }
}

/**
 * @brief 
 * 
 * @param x_pwmInerface 
 */
void mcal_pwm_channel_enable(mcal_pwmConfig_t *pwmCfg)
{
    switch (pwmCfg->timer)
    {
    case MCAL_TIMER_0:
        switch (pwmCfg->freq)
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

        break;

        /* TODO: needs rework */
    case MCAL_TIMER_1:
        switch (pwmCfg->freq)
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

        break;

    case MCAL_TIMER_2:
        switch (pwmCfg->freq)
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
            BIT_CLR(TCCR2B, 0);
            BIT_CLR(TCCR2B, 1);
            BIT_CLR(TCCR2B, 2);
            break;
        }

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
void mcal_pwm_channel_disable(mcal_pwmConfig_t *pwmCfg)
{
    switch (pwmCfg->timer)
    {
    case MCAL_TIMER_0:
        BIT_CLR(TCCR0B, 0);
        BIT_CLR(TCCR0B, 1);
        BIT_CLR(TCCR0B, 2);
        break;

    case MCAL_TIMER_1:
        BIT_CLR(TCCR1B, 0);
        BIT_CLR(TCCR1B, 1);
        BIT_CLR(TCCR1B, 2);
        break;

    case MCAL_TIMER_2:
        BIT_CLR(TCCR2B, 0);
        BIT_CLR(TCCR2B, 1);
        BIT_CLR(TCCR2B, 2);
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
