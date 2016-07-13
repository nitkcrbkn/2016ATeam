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
 */

#ifndef __AB_H
#define __AB_H

#include <stdint.h>

typedef struct{
  uint8_t add;
  uint8_t dat;
}DD_ABHand_t;

/*
 * [data(7:0)][~data(7:0)]
 *
 * ２バイト送信。dataのそれぞれのビットに各電磁弁のOnOff情報を入れる。
 * 開放、未使用時は0とする。1でOnとなる。
 */
int DD_send2AB(DD_ABHand_t *dab);

/*
 * 表示内容
 * AB(Add:hex):[data:binary]
 */
void DD_ABHandPrint(DD_ABHand_t *dab);

#endif
