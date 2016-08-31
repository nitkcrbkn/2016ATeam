/* ===Kisarazu RBKN Library===
 *
 * autor          : Wakabayashi
 * version        : v0.10
 * last update    : 20160831
 *
 * **overview***
 * エンコーダーの値を取得する。
 *
 * TIM3及びTIM4を使用
 */
#include "DD_ENCODER.h"

static int32_t encval1 = 0;
static int32_t encval2 = 0;

int DD_InitEncoder1(void){
  return MW_EncoderInit(ENCODER1ID);
}

int DD_InitEncoder2(void){
  return MW_EncoderInit(ENCODER2ID);
}

int DD_encoder1update(void){  
  encval1 += (int32_t)MW_GetEncoderVal(ENCODER1ID);
  MW_WriteEncoderVal(ENCODER1ID,0);
    
  return EXIT_SUCCESS;
}

int DD_encoder2update(void){  
  encval2 += (int32_t)MW_GetEncoderVal(ENCODER2ID);
  MW_WriteEncoderVal(ENCODER2ID,0);
    
  return EXIT_SUCCESS;
}

int32_t DD_encoder1Get_int32(void){  
  return encval1;
}

int32_t DD_encoder2Get_int32(void){  
  return encval2;
}

int DD_encoderprint(void){
  MW_printf("ENC[%5d][%5d]\n",encval1,encval2);
  return EXIT_SUCCESS;
}
    
