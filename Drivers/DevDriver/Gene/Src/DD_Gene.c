/* ===Kisarazu RBKN Library===
 *
 * autor          : Oishi
 * version        : v0.10
 * last update    : 20160703
 *
 * **overview***
 * 共通部分のコードと他のレイヤーとの通信部分
 *
 * ・I2Cのみのサポート
 */
#include <stdio.h>
#include <stdlib.h>
#include "MW_I2C.h"
#include "DD_Gene.h"
#include "DD_MD.h"
#include "DD_AB.h"

/*Address Definition*/
#if DD_NUM_OF_MD
DD_MD_DRI_t h_md_g[DD_NUM_OF_MD]={
  {0x10,//address
   0,//default duty
   D_MMOD_FREE,//mode
  },
};
#endif
#if DD_NUM_OF_AB
DD_AB_DRI_t h_ab_g[DD_NUM_OF_AB]={
  {0x30,//address
   0x00,//data
  },
};
#endif

int DD_I2CSend(uint8_t add,uint8_t *data,uint8_t size){
  return MW_I2C1Transmit(add,data,size);
}

int DD_Tasks(void){
  int i;
  int ret;
#if DD_NUM_OF_MD
  for(i=0;i<DD_NUM_OF_MD;i++){
    ret = DD_Send2MD(&h_md_g[i]);
    if(ret)return ret;
  }
#endif
#if DD_NUM_OF_AB
  for(i=0;i<DD_NUM_OF_AB;i++){
    ret = DD_Send2AB(&h_ab_g[i]);
    if(ret)return ret;
  }
#endif
  return EXIT_SUCCESS;
}
