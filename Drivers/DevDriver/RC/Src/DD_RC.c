#include "DD_RC.h"
#include "DD_RCDefinition.h"
#include <stdlib.h>
#include <stdint.h>
#include "message.h"
#include "SystemTaskManager.h"
#include "MW_USART.h"

static volatile int is_not_avaiable = 8;
static volatile uint8_t correct[RC_DATA_NUM];

int DD_RCInit(uint8_t rc_data[RC_DATA_NUM], uint32_t timeout){
  uint32_t remaining;
  int ret;

  MW_USARTSetBaudRate(USART3ID, _RC_BAUD);
  ret = MW_USARTInit(USART3ID);
  if( ret ){
    return ret;
  }

  remaining = g_SY_system_counter + timeout;

  MW_USART3ReceiveMult(RC_DATA_NUM, rc_data);
  while( remaining > g_SY_system_counter && is_not_avaiable ){
  }

  if( is_not_avaiable ){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void DD_RCTask(const uint8_t rc_data[RC_DATA_NUM], uint8_t out_data[RC_DATA_NUM]){
  int i;
  if( is_not_avaiable ){
    is_not_avaiable--;
    for( i = 0; i < RC_DATA_NUM; i++ ){
      correct[rc_data[i] >> 5] += rc_data[i] & ( 0xFF >> 3 );
    }
    if( !is_not_avaiable ){
      for( i = 0; i < 4; i++ ){
        /*calc average*/
        correct[i + 4] <<= 3;
      }
    }
  }else  {
    for( i = 0; i < RC_DATA_NUM; i++ ){
      out_data[rc_data[i] >> 5] = rc_data[i] & ( 0xFF >> 3 );
    }
  }
  MW_USART3ReceiveMult(8, rc_data);
}

int DD_RCGetLX(uint8_t data[RC_DATA_NUM]){
  return data[__RC_LX] - correct[__RC_LX];
}

int DD_RCGetLY(uint8_t data[RC_DATA_NUM]){
  return data[__RC_LY] - correct[__RC_LY];
}

int DD_RCGetRX(uint8_t data[RC_DATA_NUM]){
  return data[__RC_RX] - correct[__RC_RX];
}

int DD_RCGetRY(uint8_t data[RC_DATA_NUM]){
  return data[__RC_RY] - correct[__RC_RY];
}

int DD_RCPrint(uint8_t data[RC_DATA_NUM]){
  if( is_not_avaiable ){
    return EXIT_FAILURE;
  }
  MW_printf("RC btn:[");
  if( __RC_ISPRESSED_UP(data)){
    MW_printf("Up,");
  }
  if( __RC_ISPRESSED_LEFT(data)){
    MW_printf("Lf,");
  }
  if( __RC_ISPRESSED_DOWN(data)){
    MW_printf("Dw,");
  }
  if( __RC_ISPRESSED_RIGHT(data)){
    MW_printf("Ri,");
  }

  if( __RC_ISPRESSED_CIRCLE(data)){
    MW_printf("Cr,");
  }
  if( __RC_ISPRESSED_TRIANGLE(data)){
    MW_printf("Tr,");
  }
  if( __RC_ISPRESSED_SQARE(data)){
    MW_printf("Sq,");
  }
  if( __RC_ISPRESSED_CROSS(data)){
    MW_printf("Cs,");
  }

  if( __RC_ISPRESSED_L1(data)){
    MW_printf("L1,");
  }
  if( __RC_ISPRESSED_L2(data)){
    MW_printf("L2,");
  }
  if( __RC_ISPRESSED_R1(data)){
    MW_printf("R1,");
  }
  if( __RC_ISPRESSED_R2(data)){
    MW_printf("R2,");
  }

  MW_printf("]\n");
  MW_printf("RC analog[%d,%d][%d,%d]\n",
            DD_RCGetLX(data),
            DD_RCGetLY(data),
            DD_RCGetRX(data),
            DD_RCGetRY(data)
            );

  return EXIT_SUCCESS;
} /* DD_RCPrint */

