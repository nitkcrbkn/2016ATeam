#ifndef __APP_H
#define __APP_H


#include "DD_RC.h"
#include "DD_MD.h"

/*NO Device mode*/
#define _NO_DEVICE 0

/*モータを反転させるか(１で反転) */
/* 駆動のモータの回転の向きを揃えるために使用 */
#define _IS_REVERSE_DRIL 1
#define _IS_REVERSE_DRIR 0
#define _IS_REVERSE_DRIB 0
#define _IS_REVERSE_PSH 0

#define DD_NUM_OF_MD 4
#define DD_NUM_OF_AB 1

#define ROB1_DRIL 0
#define ROB1_DRIR 1
#define ROB1_DRIB 2
#define ROB1_PSH 3

#define AB 0

#define LIFTL (1<<0)
#define LIFTR (1<<1)
#define PNCHL (1<<2)
#define PNCHR (1<<3)

#define CENTRAL_THRESHOLD 5

#define MD_MAX_DUTY 9999

#define MD_MAX_DUTY_DRIL MD_MAX_DUTY
#define MD_MAX_DUTY_DRIR MD_MAX_DUTY
#define MD_MAX_DUTY_DRIB MD_MAX_DUTY
#define MD_MAX_DUTY_PSH MD_MAX_DUTY

#define MD_GAIN_DRIL ( MD_MAX_DUTY_DRIL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIR ( MD_MAX_DUTY_DRIR / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIB ( MD_MAX_DUTY_DRIB / DD_RC_ANALOG_MAX )

int appTask(void);
int appInit(void);

#endif






