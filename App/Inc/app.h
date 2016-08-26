#ifndef __APP_H
#define __APP_H

#include "DD_RC.h"
#include "DD_MD.h"

/*NO Device mode*/
#define _NO_DEVICE 1

/*モータを反転させるか(１で反転) */
/* 駆動のモータの回転の向きを揃えるために使用 */
#define _IS_REVERSE_DRIL 1
#define _IS_REVERSE_DRIR 0

/* モータ、電磁弁の個数 */
#define DD_NUM_OF_MD 2
#define DD_NUM_OF_AB 0

/* 各モータのインデックス */
#define ROB1_DRIL 0
#define ROB1_DRIR 1

/* コントローラのスティックの補正値 */
#define CENTRAL_THRESHOLD 5

#define MD_MAX_DUTY_DRIL DD_MD_MAX_DUTY
#define MD_MAX_DUTY_DRIR DD_MD_MAX_DUTY

#define MD_GAIN_DRIL ( MD_MAX_DUTY_DRIL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIR ( MD_MAX_DUTY_DRIR / DD_RC_ANALOG_MAX )

typedef struct {
  int rising_val; /* 立ち上がり値 */
  int falling_val; /* 立ち下がり値 */
} inc_val_t;

int appTask(void);
int appInit(void);

#endif






