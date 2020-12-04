#ifndef OS_H__
#define OS_H__

#include "mcal.h"
#include "config.h"
#include "utils.h"

void os_scheduler_init(void);
void os_scheduler_start(void);
int8_t os_scheduler_task_add(p_fn_t function, void *args, uint32_t delay_ms, uint32_t periodicity_ms);
void os_scheduler_task_remove(uint8_t task_id);
#endif   // !OS_H__