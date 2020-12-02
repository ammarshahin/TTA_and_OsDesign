#ifndef MCAL_H
#define MCAL_H

#include <stdint-gcc.h>
#include "bsp.h"
#include "def.h"

typedef enum
{
    MCAL_ENABLE = 1,
    MCAL_DISABLE = 0
} mcal_functionalStateEnum_t;

typedef enum
{
    MCAL_SUCCESS = 0,
    MCAL_ERROR = 1
} mcal_resultStateEnum_t;

typedef enum
{
    MCAL_BUSY = 0,
    MCAL_DONE = 1
} mcal_workStateEnum_t;

#if MCU == TMPM370
#include "mcal_tmpm370.h"
#elif MCU == ATMEGA328P
#include "mcal_atmega328p.h"
#endif

#endif
