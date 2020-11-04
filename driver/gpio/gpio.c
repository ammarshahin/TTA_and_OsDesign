#include "gpio.h"
#include "mcal.h"

void gpio_init(gpio_t *px_gpio)
{
    mcal_gpio_t x_mcalgpio;
    x_mcalgpio.port = px_gpio->port;
    x_mcalgpio.pin = px_gpio->pin;
    x_mcalgpio.ioState = px_gpio->ioState;

    if (px_gpio->state != GPIO_HIGH || px_gpio->state != GPIO_LOW)
    {
        px_gpio->state = GPIO_LOW;
    }

    // LOG: changed the order for the calling of the next functions.
    mcal_gpio_pinState_set(&x_mcalgpio, px_gpio->state);
    mcal_gpio_pin_init(&x_mcalgpio);
}

void gpio_pinIOState_set(gpio_port_t port, gpio_pin_t pin, gpio_ioState_t ioState)
{
    mcal_gpio_t x_mcalgpio;
    x_mcalgpio.port = port;
    x_mcalgpio.pin = pin;
    x_mcalgpio.ioState = ioState;

    mcal_gpio_pinIOState_set(&x_mcalgpio, x_mcalgpio.ioState);
}

void gpio_pinInPullupState_set(gpio_port_t port, gpio_pin_t pin, bool_t pullupEnable)
{
    mcal_gpio_pinInPullupState((mcal_gpio_portEnum_t)port, (mcal_gpio_pinEnum_t)pin, pullupEnable);
}

void gpio_pinState_set(gpio_port_t port, gpio_pin_t pin, gpio_state_t x_state)
{
    mcal_gpio_t x_mcalgpio;
    x_mcalgpio.port = port;
    x_mcalgpio.pin = pin;
    //x_mcalgpio.ioState = px_gpio->ioState;

    mcal_gpio_pinState_set(&x_mcalgpio, x_state);
}

uint8_t gpio_pinState_get(gpio_port_t port, gpio_pin_t pin)
{
    mcal_gpio_t x_mcalgpio;
    x_mcalgpio.port = port;
    x_mcalgpio.pin = pin;

    return mcal_gpio_pinState_get(&x_mcalgpio);
}
