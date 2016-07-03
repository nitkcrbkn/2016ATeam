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
#include "DD_Gene.h"

int DD_I2CSend(uint8_t add,uint8_t *data,uint8_t size){
  return MW_I2C1Transmit(add,data,size);
}
