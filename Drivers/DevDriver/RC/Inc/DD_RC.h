#ifndef __RC_H
#define __RC_H
#include <stdint.h>

extern uint8_t g_rc_dat[8];

int DD_RCInit(uint8_t setdata[8],uint32_t timeout);
void DD_RCTask(const uint8_t rcv_data[8],uint8_t setdata[8]);
int DD_RCPrint(uint8_t data[8]);

#endif
