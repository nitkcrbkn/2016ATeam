#ifndef __RC_H
#define __RC_H
#include <stdint.h>

#define RC_DATA_NUM 8

int DD_RCInit(uint8_t setdata[RC_DATA_NUM],uint32_t timeout);
void DD_RCTask(uint8_t rcv_data[RC_DATA_NUM],uint8_t setdata[RC_DATA_NUM]);
int DD_RCPrint(volatile uint8_t data[RC_DATA_NUM]);

/*アナログデータ取得*/
int DD_RCGetLX(volatile uint8_t data[RC_DATA_NUM]);
int DD_RCGetLY(volatile uint8_t data[RC_DATA_NUM]);
int DD_RCGetRX(volatile uint8_t data[RC_DATA_NUM]);
int DD_RCGetRY(volatile uint8_t data[RC_DATA_NUM]);

#define DD_RC_ANALOG_MAX 16
  
#endif
