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
 */

#ifndef __MD_H
#define __MD_H
#include <stdint.h>

#define DD_MD_MAX_DUTY 10000

typedef enum{
  D_MMOD_FREE=0,
  D_MMOD_FORWARD=1,
  D_MMOD_BACKWARD=2,
  D_MMOD_BRAKE=3,
} DD_MDMode_t;

typedef struct{
  uint8_t add;
  uint16_t duty;
  DD_MDMode_t mode;
}DD_MDHand_t;

/*
 * ２バイト送信。
 * [0(7:4)|dir(3:2)|duty[9:8](1:0)][duty[7:0](7:0)]
 *
 * dirは0...free,1...forward,2...back,3...brake
 */
int DD_send2MD(DD_MDHand_t *dmd);


/*
 *MD handlerを表示。
 *
 *MD(Add:hex):[Fr,Fw,Bw,Br],[duty:dec]
 */
void DD_MDHandPrint(DD_MDHand_t *dmd);

#endif
