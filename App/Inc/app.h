#ifndef __APP_H
#define __APP_H


/* モータ、電磁弁の個数 */
#define DD_NUM_OF_MD 3
#define DD_NUM_OF_AB 0

#define DD_USE_ENCODER1 0
#define DD_USE_ENCODER2 0
#define DD_NUM_OF_SV 0

#include "DD_RC.h"
#include "DD_MD.h"
#include "DD_SV.h"

/*NO Device mode*/
#define _NO_DEVICE 0

/*モータを反転させるか(１で反転) */
/* 駆動のモータの回転の向きを揃えるために使用 */
#define _IS_REVERSE_MTRL 0
#define _IS_REVERSE_DRIS 0
#define _IS_REVERSE_XPNS 1

/* 各モータのインデックス */
#define ROB1_MTRL 0
#define ROB1_DRIS 1
#define ROB1_XPNS 2

/* コントローラのスティックの補正値 */
#define CENTRAL_THRESHOLD 5

#define MD_MAX_DUTY 9999

#define MD_MAX_DUTY_MTRL (int) (MD_MAX_DUTY * 0.6)
#define MD_MAX_DUTY_DRIS MD_MAX_DUTY
#define MD_MAX_DUTY_XPNS MD_MAX_DUTY

#define MD_GAIN_MTRL ( MD_MAX_DUTY_MTRL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIS ( MD_MAX_DUTY_DRIS / DD_RC_ANALOG_MAX )

int appTask(void);
int appInit(void);

#endif



