#ifndef __APP_H
#define __APP_H

#include "DD_RC.h"
#include "DD_MD.h"

int appTask(void);

#define MECHA1_MD1 0
#define MECHA1_MD2 1
#define CENTRAL_THRESHOLD 3


#define MD_GAIN ( DD_MD_MAX_DUTY / DD_RC_ANALOG_MAX )

#endif
