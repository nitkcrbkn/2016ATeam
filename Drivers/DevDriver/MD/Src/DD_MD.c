/* ===Kisarazu RBKN Library===
 *
 * autor          : Oishi
 * version        : v0.10
 * last update    : 20160703
 *
 * **overview***
 * MDの通信プロトコルを定める。
 *
 * ・I2Cのみのサポート
 *
 * ２バイト送信。
 * [0(7:4)|dir(3:2)|duty[9:8](1:0)][duty[7:0](7:0)]
 *
 * dutyは0...free,1...forward,2...back,3...brake
 */
#include <stdint.h>
#include "message.h"
#include "DD_MD.h"
#include "DD_Gene.h"

int DD_Send2MD(DD_MD_DRI_t *dmd){
  uint8_t data[2];
  const uint8_t sizeof_data = 2;

  //Merge Data merge
  data[1] = dmd->duty>>8|
    (uint8_t)dmd->mode<<2;
  data[0]=dmd->duty;

  MW_printf("MD(%02x):[",dmd->add);
  switch(dmd->mode){
  case D_MMOD_FREE:
    MW_printf("Fr");
    break;
  case D_MMOD_FORWARD:
    MW_printf("Fw");
    break;
  case D_MMOD_BACKWARD:
    MW_printf("Bw");
    break;
  case D_MMOD_BRAKE:
    MW_printf("Br");
    break;
  }
  MW_printf("],[%d]\n",dmd->duty);

  //Send data
  return DD_I2CSend(dmd->add,data,sizeof_data);
}
