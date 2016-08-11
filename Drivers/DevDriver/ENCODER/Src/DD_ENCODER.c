/* ===Kisarazu RBKN Library===
 *
 * autor          : Wakabayashi
 * version        : v0.10
 * last update    : 20160811
 *
 * **overview***
 * エンコーダーの値を取得する。
 *
 * TIM3及びTIM4を使用
 */
#include "message.h"
#include "MW_ENCODER.h"

int32_t DD_InitEncoder1(void){
  return MW_EncoderInit(ENCODER1ID);
}

int32_t DD_InitEncoder2(void){
  return MW_EncoderInit(ENCODER2ID);
}

uint16_t DD_GetEncoder1(void){
  return MW_GetEncoderVal(ENCODER1ID);
}

uint16_t DD_GetEncoder2(void){
  return MW_GetEncoderVal(ENCODER2ID);
}

uint16_t DD_DiffEncoder1(void){
  static uint16_t pre = 0;
  uint16_t now,difference;

  now = MW_GetEncoderVal(ENCODER1ID);//get value of counter now
  difference = pre - now;//get difference between the last time
  pre = now;//save value

  MW_printf("Encoder1Difference(%02d):[", difference);
  
  return difference;
}
  
uint16_t DD_DiffEncoder2(void){
  static uint16_t pre = 0;
  uint16_t now,difference;

  now = MW_GetEncoderVal(ENCODER2ID);//get value of counter now
  difference = pre - now;//get difference between the last time
  pre = now;//save value
  
  MW_printf("Encoder2Difference(%02d):[", difference);  

  return difference;
}
  
