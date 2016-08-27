#ifndef __APP_H
#define __APP_H

/* モータ、電磁弁の個数 */
#define DD_NUM_OF_MD 6
#define DD_NUM_OF_AB 0
#define DD_NUM_OF_SV 0

#include "DD_RC.h"
#include "DD_MD.h"
#include "DD_SV.h"

/*NO Device mode*/
#define _NO_DEVICE 0

/* モータを反転させるか(１で反転) */
/* 駆動のモータの回転の向きを揃えるために使用 */
#define _IS_REVERSE_DRIL 1
#define _IS_REVERSE_DRIR 0
#define _IS_REVERSE_DRIB 1
#define _IS_REVERSE_ARMT 0
#define _IS_REVERSE_ARME 0
#define _IS_REVERSE_ARMS 0

/* 各モータのインデックス */
#define ROB0_DRIL 0
#define ROB0_DRIR 1
#define ROB0_DRIB 2
#define ROB0_ARMT 3
#define ROB0_ARME 4
#define ROB0_ARMS 5

/* コントローラのスティックの補正値 */
#define CENTRAL_THRESHOLD 5

#define MD_MAX_DUTY 9999

#define MD_MAX_DUTY_DRIL MD_MAX_DUTY
#define MD_MAX_DUTY_DRIR MD_MAX_DUTY
#define MD_MAX_DUTY_DRIB MD_MAX_DUTY
#define MD_MAX_DUTY_ARMT MD_MAX_DUTY
#define MD_MAX_DUTY_ARME MD_MAX_DUTY
#define MD_MAX_DUTY_ARMS MD_MAX_DUTY

#define MD_GAIN_DRIL ( MD_MAX_DUTY_DRIL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIR ( MD_MAX_DUTY_DRIR / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIB ( MD_MAX_DUTY_DRIB / DD_RC_ANALOG_MAX )
#define MD_GAIN_ARMT ( MD_MAX_DUTY_ARMT / DD_RC_ANALOG_MAX )
#define MD_GAIN_ARME ( MD_MAX_DUTY_ARME / DD_RC_ANALOG_MAX )
#define MD_GAIN_ARMS ( MD_MAX_DUTY_ARMS / DD_RC_ANALOG_MAX )

typedef enum{
  OPE_MODE_A,
  OPE_MODE_B,
} ope_mode_t;;

typedef struct {
  int rising_val; /* 立ち上がり値 */
  int falling_val; /* 立ち下がり値 */
} inc_val_t;

int appTask(void);
int appInit(void);

#endif



