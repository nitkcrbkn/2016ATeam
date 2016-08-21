#ifndef __APP_H
#define __APP_H


#include "DD_RC.h"
#include "DD_MD.h"

/*NO Device mode*/
#define _NO_DEVICE 0

/*モータを反転させるか(１で反転) */
#define _IS_REVERSE_DRIL 1
#define _IS_REVERSE_DRIR 0

#define DD_NUM_OF_MD 2
#define DD_NUM_OF_AB 0

#define ROB1_DRIL 0
#define ROB1_DRIR 1

#define CENTRAL_THRESHOLD 5

#define MD_MAX_DUTY_DRIL DD_MD_MAX_DUTY
#define MD_MAX_DUTY_DRIR DD_MD_MAX_DUTY

#define MD_GAIN_DRIL ( MD_MAX_DUTY_DRIL / DD_RC_ANALOG_MAX )
#define MD_GAIN_DRIR ( MD_MAX_DUTY_DRIR / DD_RC_ANALOG_MAX )

int appTask(void);
int appInit(void);

#endif






