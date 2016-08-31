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

#ifndef DD_ENCODER_H
#define DD_ENCODER_H

#include <stdint.h>
#include "MW_ENCODER.h"
#include "DD_Gene.h"
#include "message.h"
/*
 *エンコーダーモジュールの初期化
 *引数・・・無し
 *返り値・・・EXIT_SUCCESS:初期化成功
 *　　　　　　EXIT_FAILURE:初期化失敗
 */
int DD_InitEncoder1(void);
int DD_InitEncoder2(void);

int DD_encoder1update(void);
int DD_encoder2update(void);

/*
 *エンコーダーの現在値を取得します。
 *引数・・・無し
 *返り値・・・エンコーダーの現在値
 */
int32_t DD_encoder1Get_int32(void);
int32_t DD_encoder2Get_int32(void);

int DD_encoderprint(void);

#endif
