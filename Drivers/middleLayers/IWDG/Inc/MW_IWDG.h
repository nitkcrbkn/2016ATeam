/*
 * MW_IWDG.h
 *
 *  Created on: 2016/06/11
 *      Author: evaota
 */

#ifndef MIDLEWARE_INC_MW_IWDG_H_
#define MIDLEWARE_INC_MW_IWDG_H_

#include "stm32f1xx_hal.h"
#include <stdlib.h>

void MW_SetIWDGPrescaler(uint32_t Prescaler);
void MW_SetIWDGReload(uint32_t Reload);


int MW_IWDGInit(void);

void MW_IWDGClr(void);
uint32_t MW_IWDGStatus(void);

#endif /* MIDLEWARE_INC_MW_IWDG_H_ */
