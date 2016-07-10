#ifndef __SYSTEM_TASKS_MANAGER_H
#define __SYSTEM_TASKS_MANAGER_H

#include "DD_RC.h"
#define _INTERVAL_MS 10
#define _MESSAGE_INTERVAL_MS 30

#define _I2C_SPEED_BPS 200000

extern volatile uint32_t g_SY_system_counter;
extern volatile uint8_t g_rc_data[RC_DATA_NUM];

#endif
