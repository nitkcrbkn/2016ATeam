#ifndef __APP_H
#define __APP_H


/* モータ、電磁弁の個数 */
#define DD_NUM_OF_MD 7
#define DD_NUM_OF_AB 1

#define DD_USE_ENCODER1 0
#define DD_USE_ENCODER2 0
#define DD_NUM_OF_SV 0

#include "DD_RC.h"
#include "DD_MD.h"
#include "DD_SV.h"

/*NO Device mode*/
#define _NO_DEVICE 1

/* モータを反転させるか(１で反転) */
/* 駆動のモータの回転の向きを揃えるために使用 */
#define _IS_REVERSE_DRIL 0
#define _IS_REVERSE_DRIR 1
#define _IS_REVERSE_DRIB 1
#define _IS_REVERSE_ARMT 1
#define _IS_REVERSE_ARME 1
#define _IS_REVERSE_ARMS 0

/* 各モータのインデックス */
#define ROB0_DRIL 0
#define ROB0_DRIR 1
#define ROB0_DRIBF 2
#define ROB0_DRIBB 3
#define ROB0_ARMT 4
#define ROB0_ARME 5
#define ROB0_ARMS 6

/* 電磁弁のインデックス */
#define ROB0_VAC 0

/* 電磁弁のチャンネル */
#define VAC0  ( 1 << 0 )
#define VAC1  ( 1 << 1 )
#define VAC2  ( 1 << 2 )
#define VAC3  ( 1 << 3 )

/* コントローラのスティックの補正値 */
#define CENTRAL_THRESHOLD 5

#define MD_MAX_DUTY 9999

#define MD_MAX_DUTY_DRIL (int) (MD_MAX_DUTY * g_adjust.DRILadjust.value / 100)
#define MD_MAX_DUTY_DRIR (int) (MD_MAX_DUTY * g_adjust.DRIRadjust.value / 100)
#define MD_MAX_DUTY_DRIBF (int) (MD_MAX_DUTY * g_adjust.DRIBFadjust.value / 100)
#define MD_MAX_DUTY_DRIBB (int) (MD_MAX_DUTY * g_adjust.DRIBBadjust.value / 100)
#define MD_MAX_DUTY_ARMT MD_MAX_DUTY
#define MD_MAX_DUTY_ARME MD_MAX_DUTY
#define MD_MAX_DUTY_ARMS MD_MAX_DUTY

#define MD_GAIN_DRIL ( MD_MAX_DUTY_DRIL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIR ( MD_MAX_DUTY_DRIR / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIBF ( MD_MAX_DUTY_DRIBF / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIBB ( MD_MAX_DUTY_DRIBB / DD_RC_ANALOG_MAX )
#define MD_GAIN_ARMT ( MD_MAX_DUTY_ARMT / DD_RC_ANALOG_MAX )
#define MD_GAIN_ARME ( MD_MAX_DUTY_ARME / DD_RC_ANALOG_MAX )
#define MD_GAIN_ARMS ( MD_MAX_DUTY_ARMS / DD_RC_ANALOG_MAX )

/*アームの後端リミットスイッチは押されているか*/
#define _LIMITSW_ARM_BACK_GPIOxID GPIOCID
#define _LIMITSW_ARM_BACK_GPIOPIN GPIO_PIN_0
#define _IS_PRESSED_LIMITSW_ARM_BACK() ( !( MW_GPIORead(_LIMITSW_ARM_BACK_GPIOxID, _LIMITSW_ARM_BACK_GPIOPIN)))

typedef enum {
  OPE_MODE_A,
  OPE_MODE_B,
} ope_mode_t;

int appTask(void);
int appInit(void);

#endif


