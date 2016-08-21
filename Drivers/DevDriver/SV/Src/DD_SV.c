/* ===Kisarazu RBKN Library===
 *
 * autor          : Wakabayashi
 * version        : v0.10
 * last update    : 20160821
 *
 * **overview***。
 *
 * サポートデバイス:NXP社PCA9685
 */
#include <stdint.h>
#include "message.h"
#include "DD_Gene.h"
#include "DD_SV.h"

static uint8_t SV_Read8(uint8_t addr) {
  /* Send data */
  DD_I2CSend(_i2caddr, addr, 1);
  
  /* Receive data */
  DD_I2CReceive(_i2caddr, d, 1);
}

static uint32_t SV_Write8(uint8_t addr, uint8_t d) {
  uint8_t data[2];
  
  data[0] = addr;
  data[1] = d;
  /* Send data */
  return DD_I2CSend(_i2caddr, data, 2);

}

uint32_t SV_Init(void) {
  write8(PCA9685_MODE1, 0x0);
}

uint32_t SV_SetPWMFreq(float freq) {
  //Serial.print("Attempting to set freq ");
  //Serial.println(freq);
  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
  //Serial.print("Estimated pre-scale: ");
  //Serial.println(prescaleval);
  uint8_t prescale = floor(prescaleval + 0.5);
  //Serial.print("Final pre-scale: ");
  //Serial.println(prescale);
  
  uint8_t oldmode = read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep

  write8(PCA9685_MODE1, newmode); // go to sleep
  write8(PCA9685_PRESCALE, prescale); // set the prescaler
  write8(PCA9685_MODE1, oldmode);
  delay(5);
  write8(PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
                                          // This is why the beginTransmission below was not working.
  //Serial.print("Mode now 0x");
  //Serial.println(read8(PCA9685_MODE1), HEX);
}

uint32_t SV_SetPWM(uint8_t num, uint16_t on, uint16_t off) {
  //Serial.print("Setting PWM ");
  //Serial.print(num);
  //Serial.print(": ");
  //Serial.print(on);
  //Serial.print("->");
  //Serial.println(off);
  uint8_t data[5];

  data[0] = LED0_ON_L+4*num;
  data[1] = on;
  data[2] = on>>8;
  data[3] = off;
  data[4] = off>>8;
  
  /* Send data */
  DD_I2CSend(_i2caddr, data, 5);
  
}

