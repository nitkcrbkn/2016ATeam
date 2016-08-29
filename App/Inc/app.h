#ifndef __APP_H
#define __APP_H

/*NO Device mode*/
#define _NO_DEVICE 1

int appTask(void);
int appInit(void);

#define DD_NUM_OF_MD 2
#define DD_NUM_OF_AB 1

#define DD_USE_ENCODER1 1
#define DD_USE_ENCODER1 1
#define DD_NUM_OF_SV 0

#include "DD_RC.h"
#include "DD_MD.h"
#include "DD_SV.h"

#define MECHA1_MD1 0
#define MECHA1_MD2 1
#define CENTRAL_THRESHOLD 4

#define AB0 (1<<0)
#define AB1 (1<<1)

#define MD_GAIN ( DD_MD_MAX_DUTY / DD_RC_ANALOG_MAX )

#endif
