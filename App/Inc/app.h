#ifndef __APP_H
#define __APP_H


/* モータ、電磁弁の個数 */
#define DD_NUM_OF_MD 4
#define DD_NUM_OF_AB 1

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
#define _IS_REVERSE_DRIL 0
#define _IS_REVERSE_DRIR 1
#define _IS_REVERSE_DRIB 0
#define _IS_REVERSE_PSH 0

/* 各モータのインデックス */
#define ROB1_DRIL 0
#define ROB1_DRIR 1
#define ROB1_DRIB 2
#define ROB1_PSH 3

/* 電磁弁のインデックス */
#define ROB1_AB 0

/* 電磁弁のチャンネル */
#define LIFTL (1<<0)
#define LIFTR (1<<1)
#define PNCHL (1<<2)
#define PNCHR (1<<3)

/* コントローラのスティックの補正値 */
#define CENTRAL_THRESHOLD 5

#define MD_MAX_DUTY 9999

#define MD_MAX_DUTY_DRIL MD_MAX_DUTY
#define MD_MAX_DUTY_DRIR MD_MAX_DUTY
#define MD_MAX_DUTY_DRIB MD_MAX_DUTY
#define MD_MAX_DUTY_PSH MD_MAX_DUTY

#define MD_GAIN_DRIL ( MD_MAX_DUTY_DRIL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIR ( MD_MAX_DUTY_DRIR / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIB ( MD_MAX_DUTY_DRIB / DD_RC_ANALOG_MAX )

typedef struct {
  int rising_val; /* 立ち上がり値 */
  int falling_val; /* 立ち下がり値 */
} tc_slope_lim_t;

int appTask(void);
int appInit(void);

#endif






