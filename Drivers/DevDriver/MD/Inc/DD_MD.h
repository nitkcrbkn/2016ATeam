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
#ifndef __MD_H
#define __MD_H
#include <stdint.h>

typedef enum{
  D_MMOD_FREE=0,
  D_MMOD_FORWARD=1,
  D_MMOD_BACKWARD=2,
  D_MMOD_BRAKE=3,
} DD_MD_MODE_t;

typedef struct{
  uint8_t add;
  uint16_t duty;
  DD_MD_MODE_t mode;
}DD_MD_DRI_t;

int DD_Send2MD(DD_MD_DRI_t *dmd);

#endif
