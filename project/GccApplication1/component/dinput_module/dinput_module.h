#ifndef DINPUT_MODULE_H
#define DINPUT_MODULE_H

#include "mcal.h"
#include "config.h"
#include "gpio.h"
#include "def.h"

typedef struct
{
    gpio_port_t port;
    gpio_pin_t pin;
    bool_t pullUpEn;
    uint8_t dinputID;                // id given by the app to be used later to with the getters `unique for each sw`
    uint32_t shortPressMultiplier;   // multiplies of the period (so each sw can have different update interval)
    uint32_t longPressMultiplier;
} dinputModule_t;

typedef enum
{
    DINPUT_MODULE_RELEASED,
    DINPUT_MODULE_PRESSED_SHORT_PRESS,
    DINPUT_MODULE_PRESSED_LONG_PRESS,
    DINPUT_MODULE_FAULT = -1
} dinputModule_stateEnum_t;

void dinputModule_init(dinputModule_t *x_dinputModule);
void dinputModule_update(void *arg);
dinputModule_stateEnum_t dinputModule_state_get(uint8_t u8_dinputID);
dinputModule_stateEnum_t dinputModule_edgeState_get(uint8_t u8_dinputID);
dinputModule_stateEnum_t dinputModule_fastEdgeState_get(uint8_t u8_dinputID);
void dinputModule_shortPressMultiplier_set(uint8_t u8_dinputID, uint32_t u32_newMultiplier);
void dinputModule_longPressMultiplier_set(uint8_t u8_dinputID, uint32_t u32_newMultiplier);

#endif
