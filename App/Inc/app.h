#ifndef __APP_H
#define __APP_H


/* モータ、電磁弁の個数 */
#define DD_NUM_OF_MD 5
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
#define _IS_REVERSE_DRIL 0
#define _IS_REVERSE_DRIR 1
#define _IS_REVERSE_DRIBL 0
#define _IS_REVERSE_DRIBR 0
#define _IS_REVERSE_ARM 0

/* 各モータのインデックス */
#define ROB1_DRIL 0
#define ROB1_DRIR 1
#define ROB1_DRIBL 2
#define ROB1_DRIBR 3
#define ROB1_ARM 4

/* コントローラのスティックの補正値 */
#define CENTRAL_THRESHOLD 5

#define MD_MAX_DUTY 9999

#define MD_MAX_DUTY_DRIL (int) (MD_MAX_DUTY * g_adjust.DRILadjust.value / 100)
#define MD_MAX_DUTY_DRIR (int) (MD_MAX_DUTY * g_adjust.DRIRadjust.value / 100)
#define MD_MAX_DUTY_DRIBL MD_MAX_DUTY
#define MD_MAX_DUTY_DRIBR MD_MAX_DUTY
#define MD_MAX_DUTY_ARM MD_MAX_DUTY * 0.5

#define MD_GAIN_DRIL ( MD_MAX_DUTY_DRIL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIR ( MD_MAX_DUTY_DRIR / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIBL ( MD_MAX_DUTY_DRIBL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIBR ( MD_MAX_DUTY_DRIBR / DD_RC_ANALOG_MAX )

/*アームの先端リミットスイッチは押されているか*/
#define _LIMITSW_ARM_GPIOxID GPIOCID
#define _LIMITSW_ARM_GPIOPIN GPIO_PIN_0
#define _IS_PRESSED_LIMITSW_ARM() (!(MW_GPIORead(_LIMITSW_ARM_GPIOxID, _LIMITSW_ARM_GPIOPIN)))

int appTask(void);
int appInit(void);

#endif






