#ifndef _HEARTBEAT_H
#define _HEARTBEAT_H

#include "gpio.h"

void heartbeat_init(gpio_t *px_led);
void heartbeat_update(void *arg);

#endif
