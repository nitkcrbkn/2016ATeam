/* ===Kisarazu RBKN Library===
 *
 * autor          : Oishi
 * version        : v0.10
 * last update    : 20160703
 *
 * **overview***
 * ABの通信プロトコルを定める。
 *
 * ・I2Cのみのサポート
 * [data(7:0)][~data(7:0)]
 *
 * ２バイト送信。dataのそれぞれのビットに各電磁弁のOnOff情報を入れる。
 * 開放、未使用時は0とする。1でOnとなる。
 */

#include <stdint.h>
#include "DD_Gene.h"
#include "DD_AB.h"
#include "message.h"
int DD_Send2AB(DD_AB_DRI_t *dab){
  uint8_t data[2];
  const uint8_t sizeof_data = 2;

  //Merge data
  data[0] = dab->dat;
  data[1] = ~dab->dat;

  MW_printf("AB(%02x):[%b]",dab->add,dab->dat);

  //Send data
  return DD_I2CSend(dab->add,data,sizeof_data);
}
