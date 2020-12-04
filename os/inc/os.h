#ifndef OS_H__
#define OS_H__

#include "mcal.h"
#include "config.h"
#include "utils.h"

void os_scheduler_init(void);
void os_scheduler_start(void);
int8_t os_scheduler_task_add(p_fn_t function, void *args, uint16_t delay_ms, uint16_t periodicity_ms);
void os_scheduler_task_remove(p_fn_t task_function);
#endif   // !OS_H__