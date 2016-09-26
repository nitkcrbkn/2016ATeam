#include "DD_RC.h"
#include "DD_RCDefinition.h"
#include <stdlib.h>
#include <stdint.h>
#include "message.h"
#include "SystemTaskManager.h"
#include "MW_USART.h"

static volatile int is_not_avaiable = 8+1;
static volatile uint8_t correct[RC_DATA_NUM];

#define _RELANGE(x) (x<15?(x>-15?x:-15):15)

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

void DD_RCTask(uint8_t rc_data[RC_DATA_NUM], uint8_t out_data[RC_DATA_NUM]){
  int i;
  
  if( is_not_avaiable > 0){
    is_not_avaiable--;
    for( i = 0; i < RC_DATA_NUM; i++ ){
      correct[rc_data[i] >> 5] += rc_data[i] & ( 0xFF >> 3 );
    }
    if( !is_not_avaiable ){
      for( i = 0; i < RC_DATA_NUM; i++ ){
        /*calc average*/
        correct[i] >>= 3;
      }
    }
  }else  {
    for( i = 0; i < RC_DATA_NUM; i++ ){
      out_data[rc_data[i] >> 5] = rc_data[i] & ( 0xFF >> 3 );
    }
  }
  MW_USART3ReceiveMult(8, rc_data);
}

int DD_RCGetLX(volatile uint8_t data[RC_DATA_NUM]){
  return _RELANGE((int)data[__RC_LX] - (int)correct[__RC_LX]);
}

int DD_RCGetLY(volatile uint8_t data[RC_DATA_NUM]){
  return _RELANGE((int)data[__RC_LY] - (int)correct[__RC_LY]);
}

int DD_RCGetRX(volatile uint8_t data[RC_DATA_NUM]){
  return _RELANGE((int)data[__RC_RX] - (int)correct[__RC_RX]);
}

int DD_RCGetRY(volatile uint8_t data[RC_DATA_NUM]){
  return _RELANGE((int)data[__RC_RY] - (int)correct[__RC_RY]);
}

int DD_RCPrint(volatile uint8_t data[RC_DATA_NUM]){
  int bp;

  bp=__RC_ISPRESSED_UP(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("↑  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }
  
  bp=__RC_ISPRESSED_DOWN(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("↓  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }

  bp=__RC_ISPRESSED_RIGHT(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("→  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }
  
  bp=__RC_ISPRESSED_LEFT(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("←  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }

  bp=__RC_ISPRESSED_SQARE(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("□  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }
  
  bp=__RC_ISPRESSED_CROSS(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("X  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }  

  bp=__RC_ISPRESSED_CIRCLE(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("○  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }  

  bp=__RC_ISPRESSED_TRIANGLE(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("△  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }  

  bp=__RC_ISPRESSED_L1(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("L1  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }  

  bp=__RC_ISPRESSED_L2(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("L2  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }  

  bp=__RC_ISPRESSED_R1(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("R1  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }
  
  bp=__RC_ISPRESSED_R2(data);
  if(bp){
    MW_printf("\033[1;33m\033[40m");
  }
  MW_printf("R2  ");
  if(bp){
    MW_printf("\033[0;39m\033[49m");
  }  
  MW_printf("\n");
  
  MW_printf("(%3d,%3d),(%3d,%3d)\n",
	    DD_RCGetLX(data),
	    DD_RCGetLY(data),
	    DD_RCGetRX(data),
	    DD_RCGetRY(data)
	    );
  
  return EXIT_SUCCESS;
}
