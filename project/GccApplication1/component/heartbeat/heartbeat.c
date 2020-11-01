#include "gpio.h"

static gpio_t gx_heartbeatLED;

void heartbeat_init(gpio_t *px_led)
{
    px_led->ioState = GPIO_OUTPUT;

    gx_heartbeatLED.port = px_led->port;
    gx_heartbeatLED.pin = px_led->pin;
    gx_heartbeatLED.ioState = px_led->ioState;
    gx_heartbeatLED.state = px_led->state;

    gpio_init(&gx_heartbeatLED);
    gpio_pinState_set(gx_heartbeatLED.port, gx_heartbeatLED.pin, gx_heartbeatLED.state);
}

void heartbeat_update(void *arg)
{
    if (gx_heartbeatLED.state == GPIO_HIGH)
    {
        gx_heartbeatLED.state = GPIO_LOW;
        gpio_pinState_set(gx_heartbeatLED.port, gx_heartbeatLED.pin, gx_heartbeatLED.state);
    }
    else
    {
        gx_heartbeatLED.state = GPIO_HIGH;
        gpio_pinState_set(gx_heartbeatLED.port, gx_heartbeatLED.pin, gx_heartbeatLED.state);
    }
}
