#include "gpio.h"
#include "config.h"
#include "def.h"
#include "doutput_module.h"
#include "component_id.h"

static doutputModule_t gax_doutputModule_internalHandler[DOUTPUT_MODULES_NUM];
static uint8_t gu8_doutputModule_internalCounter = 0;

void doutputModule_init(doutputModule_t *x_doutputModule)
{
    gax_doutputModule_internalHandler[gu8_doutputModule_internalCounter].gpio.port = x_doutputModule->gpio.port;
    gax_doutputModule_internalHandler[gu8_doutputModule_internalCounter].gpio.pin = x_doutputModule->gpio.pin;
    gax_doutputModule_internalHandler[gu8_doutputModule_internalCounter].gpio.ioState = GPIO_OUTPUT;
    gax_doutputModule_internalHandler[gu8_doutputModule_internalCounter].gpio.state = x_doutputModule->gpio.state;
    gax_doutputModule_internalHandler[gu8_doutputModule_internalCounter].outputID = x_doutputModule->outputID;

    gpio_init(&(gax_doutputModule_internalHandler[gu8_doutputModule_internalCounter].gpio));
    gpio_pinState_set(gax_doutputModule_internalHandler[gu8_doutputModule_internalCounter].gpio.port, gax_doutputModule_internalHandler[gu8_doutputModule_internalCounter].gpio.pin, gax_doutputModule_internalHandler[gu8_doutputModule_internalCounter].gpio.state);

    gu8_doutputModule_internalCounter++;
}

result_t doutputModule_state_set(uint8_t u8_doutputID, gpio_state_t x_state)
{
    uint8_t i = 0;

    for (i = 0; i < gu8_doutputModule_internalCounter; i++)
    {
        if (gax_doutputModule_internalHandler[i].outputID == u8_doutputID)
        {
            gax_doutputModule_internalHandler[i].gpio.state = x_state;
            return OP_SUCCESS;
        }
    }

    return OP_FAILED;
}

gpio_state_t doutputModule_state_get(uint8_t u8_doutputID)
{
    uint8_t i = 0;

    for (i = 0; i < gu8_doutputModule_internalCounter; i++)
    {
        if (gax_doutputModule_internalHandler[i].outputID == u8_doutputID)
        {
            return gax_doutputModule_internalHandler[i].gpio.state;
        }
    }
}

void doutputModule_update(void *arg)
{
    uint8_t i = 0;
    gpio_state_t oldState, newState;

    for (i = 0; i < gu8_doutputModule_internalCounter; i++)
    {
        oldState = gax_doutputModule_internalHandler[i].gpio.state;
        gpio_pinState_set(gax_doutputModule_internalHandler[i].gpio.port, gax_doutputModule_internalHandler[i].gpio.pin, gax_doutputModule_internalHandler[i].gpio.state);

        newState = gpio_pinState_get(gax_doutputModule_internalHandler[i].gpio.port, gax_doutputModule_internalHandler[i].gpio.pin);

        if (newState != oldState)
        {
            // log_errorLog_record(moduleName, functionName, gax_doutputModule_internalHandler[i].outputID, PIN_NOT_RESPONDING)
        }
    }
}
