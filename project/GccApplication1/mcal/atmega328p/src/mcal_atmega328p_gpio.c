#include "mcal.h"
#include "utils.h"

void mcal_gpio_init(void)
{
    // do nothing
}

void mcal_gpio_pin_init(mcal_gpio_t *px_gpio)
{
    uint8_t volatile *u8_ddrReg = NULL;
    uint8_t volatile *u8_portReg = NULL;

    switch (px_gpio->port)
    {
    case MCAL_GPIO_PORTB:
        u8_ddrReg = &DDRB;
        u8_portReg = &PORTB;
        break;

    case MCAL_GPIO_PORTC:
        u8_ddrReg = &DDRC;
        u8_portReg = &PORTC;
        break;

    case MCAL_GPIO_PORTD:
        u8_ddrReg = &DDRD;
        u8_portReg = &PORTD;
        break;

    default:
        break;
    }

    switch (px_gpio->ioState)
    {
    case MCAL_GPIO_INPUT:
        BIT_CLR(*u8_ddrReg, px_gpio->pin - 1);
        break;

    case MCAL_GPIO_OUTPUT:
        BIT_SET(*u8_ddrReg, px_gpio->pin - 1);
        break;

    default:
        // do nothing
        break;
    }

    BIT_CTRL(*u8_portReg, px_gpio->pin - 1, px_gpio->pinState);
}

// output
void mcal_gpio_pinState_set(mcal_gpio_t *px_gpio, mcal_gpio_stateEnum_t x_state)
{
    uint8_t volatile *u8_portReg = NULL;
    switch (px_gpio->port)
    {
    case MCAL_GPIO_PORTB:
        u8_portReg = &PORTB;
        break;

    case MCAL_GPIO_PORTC:
        u8_portReg = &PORTC;
        break;

    case MCAL_GPIO_PORTD:
        u8_portReg = &PORTD;
        break;

    default:
        break;
    }

    BIT_CTRL(*u8_portReg, px_gpio->pin - 1, x_state);
}

// output
void mcal_gpio_portState_set(mcal_gpio_t *px_gpio, mcal_gpio_stateEnum_t x_state)
{
    uint8_t volatile *u8_portReg = NULL;
    switch (px_gpio->port)
    {
    case MCAL_GPIO_PORTB:
        u8_portReg = &PORTB;
        break;

    case MCAL_GPIO_PORTC:
        u8_portReg = &PORTC;
        break;

    case MCAL_GPIO_PORTD:
        u8_portReg = &PORTD;
        break;

    default:
        break;
    }

    *u8_portReg = x_state;
}

// input
uint8_t mcal_gpio_pinState_get(mcal_gpio_t *px_gpio)
{
    uint8_t volatile *u8_pinReg = NULL;
    switch (px_gpio->port)
    {
    case MCAL_GPIO_PORTB:
        u8_pinReg = &PINB;
        break;

    case MCAL_GPIO_PORTC:
        u8_pinReg = &PINC;
        break;

    case MCAL_GPIO_PORTD:
        u8_pinReg = &PIND;
        break;

    default:
        break;
    }

    return BIT_GET(*u8_pinReg, px_gpio->pin - 1);
}

// input
uint8_t mcal_gpio_portState_get(mcal_gpio_t *px_gpio)
{
    uint8_t volatile *u8_pinReg = NULL;
    switch (px_gpio->port)
    {
    case MCAL_GPIO_PORTB:
        u8_pinReg = &PINB;
        break;

    case MCAL_GPIO_PORTC:
        u8_pinReg = &PINC;
        break;

    case MCAL_GPIO_PORTD:
        u8_pinReg = &PIND;
        break;

    default:
        break;
    }

    return (uint8_t)*u8_pinReg;
}

void mcal_gpio_pinIOState_set(mcal_gpio_t *px_gpio, mcal_gpio_ioStateEnum_t x_ioState)
{
    uint8_t volatile *u8_ddrReg = NULL;
    switch (px_gpio->port)
    {
    case MCAL_GPIO_PORTB:
        u8_ddrReg = &DDRB;
        break;

    case MCAL_GPIO_PORTC:
        u8_ddrReg = &DDRC;
        break;

    case MCAL_GPIO_PORTD:
        u8_ddrReg = &DDRD;
        break;

    default:
        break;
    }

    switch (x_ioState)
    {
    case MCAL_GPIO_INPUT:
        BIT_CLR(*u8_ddrReg, px_gpio->pin - 1);
        break;

    case MCAL_GPIO_OUTPUT:
        BIT_SET(*u8_ddrReg, px_gpio->pin - 1);
        break;

    default:
        // do nothing
        break;
    }
}

// output
void mcal_gpio_pinState_toggle(mcal_gpio_t *px_gpio)
{
    uint8_t volatile *u8_portReg = NULL;
    switch (px_gpio->port)
    {
    case MCAL_GPIO_PORTB:
        u8_portReg = &PORTB;
        break;

    case MCAL_GPIO_PORTC:
        u8_portReg = &PORTC;
        break;

    case MCAL_GPIO_PORTD:
        u8_portReg = &PORTD;
        break;

    default:
        break;
    }

    BIT_TOG(*u8_portReg, px_gpio->pinState);
}

#if 0
void mcal_gpio_altFunction_set(mcal_gpio_t *px_gpio, mcal_gpio_alternateFnEnum_t x_func)
{
    switch (x_func)
    {
    case MCAL_GPIO_NORMAL:
        GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin-1);
        GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin-1);
        GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_3, px_gpio->pin-1);
        break;

    case MCAL_GPIO_ALT_FUNC_1:
        GPIO_EnableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin-1);
        GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin-1);
        GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_3, px_gpio->pin-1);
        break;

    case MCAL_GPIO_ALT_FUNC_2:
        GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin-1);
        GPIO_EnableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin-1);
        GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_3, px_gpio->pin-1);
        break;

    case MCAL_GPIO_ALT_FUNC_3:
        GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_1, px_gpio->pin-1);
        GPIO_DisableFuncReg(px_gpio->port, GPIO_FUNC_REG_2, px_gpio->pin-1);
        GPIO_EnableFuncReg(px_gpio->port, GPIO_FUNC_REG_3, px_gpio->pin-1);
        break;

    default:
        // do nothing
        break;
    }
}
#endif