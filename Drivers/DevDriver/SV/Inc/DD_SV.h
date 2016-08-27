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

#ifndef __DD_SV_H
#define __DD_SV_H

#include "app.h"

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

typedef struct
{
  uint8_t i2cadd;
  uint16_t val[DD_NUM_OF_SV];
}DD_SV_t;

int SV_Init(DD_SV_t *dsv);
int SV_SetRad(DD_SV_t *dsv);
int SV_print(DD_SV_t *dsv);

#endif
