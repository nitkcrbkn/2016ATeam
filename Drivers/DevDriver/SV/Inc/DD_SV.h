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
#define __MD_SV_H
/*sample code
uint8_t servonum = 0;
while(1)
{
  for(uint16_t len=SERVOMIN;len<SERVOMAX;len++)
    setPWM(servonum,0,len);
  delay(500);
  for(uint16_t len2=SERVOMAX;len2>SERVOMIN;len2--)
    setPWM(servonum,0,len2);
  delay(500);
  servonum++;
  if(servonum>7)servonum = 0;
}
*/
#include <stdint.h>

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

#endif
