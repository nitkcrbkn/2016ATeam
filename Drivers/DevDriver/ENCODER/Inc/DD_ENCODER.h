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
#include "message.h"
#include "MW_ENCODER.h"

/*
 *エンコーダーモジュールの初期化
 *引数・・・無し
 *返り値・・・EXIT_SUCCESS:初期化成功
 *　　　　　　EXIT_FAILURE:初期化失敗
 */
int32_t DD_InitEncoder1(void);
int32_t DD_InitEncoder2(void);

/*
 *エンコーダーの現在値を取得します。
 *引数・・・無し
 *返り値・・・エンコーダーの現在値(最大65535)
 */
uint16_t DD_GetEncoder1(void);
uint16_t DD_GetEncoder2(void);

/*前回実行した時の値との差を取得します。
 *引数・・・無し
 *返り値・・・前回実行した時の値との差
 */
uint16_t DD_DiffEncoder1(void);
uint16_t DD_DiffEncoder2(void);

#endif
