#ifndef DOUTPUT_MODULE_H
#define DOUTPUT_MODULE_H

#include "gpio.h"
#include "def.h"

typedef struct
{
    gpio_t gpio;
    uint8_t outputID;
} doutputModule_t;

void doutputModule_init(doutputModule_t *x_doutputModule);
result_t doutputModule_state_set(uint8_t u8_doutputID, gpio_state_t x_state);
gpio_state_t doutputModule_state_get(uint8_t u8_doutputID);
void doutputModule_update(void *arg);

#endif
