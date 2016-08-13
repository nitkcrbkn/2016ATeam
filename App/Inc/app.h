#ifndef __APP_H
#define __APP_H

/*NO Device mode*/
#define _NO_DEVICE 0

/*モータを反転させるか(１で反転) */
#define _REVERSE_DRIL 1
#define _REVERSE_DRIR 1
#define _REVERSE_DRIB 1
#define _REVERSE_PSH 0


#include "DD_RC.h"
#include "DD_MD.h"

int appTask(void);
int appInit(void);

#define DD_NUM_OF_MD 4
#define DD_NUM_OF_AB 0

#define ROB1_DRIL 0
#define ROB1_DRIR 1
#define ROB1_DRIB 2
#define ROB1_PSH 3

#define CENTRAL_THRESHOLD 5

#define MD_MAX_DUTY_DRIL DD_MD_MAX_DUTY
#define MD_MAX_DUTY_DRIR DD_MD_MAX_DUTY
#define MD_MAX_DUTY_DRIB DD_MD_MAX_DUTY
#define MD_MAX_DUTY_PSH DD_MD_MAX_DUTY

#define MD_GAIN_DRIL ( MD_MAX_DUTY_DRIL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIR ( MD_MAX_DUTY_DRIR / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIB ( MD_MAX_DUTY_DRIB / DD_RC_ANALOG_MAX )

#endif






