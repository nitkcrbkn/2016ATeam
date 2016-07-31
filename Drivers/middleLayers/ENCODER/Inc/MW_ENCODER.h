/*
 * MW_ENCODER.h
 *
 *  Created on: 2016/06/12
 *      Author: evaota
 */

#ifndef MIDLEWARE_INC_MW_ENCODER_H_
#define MIDLEWARE_INC_MW_ENCODER_H_

#include "stm32f1xx_hal.h"
#include <stdlib.h>

typedef enum
  {
    ENCODER1ID = 0,
    ENCODER2ID = 1
  }encoderid_t;

/**Sample code(main)
 *
 *
 *
 *
 *
 */

/******************************* TIM ID *******************************/
#define IS_ENCODER_ID(ID) (((ID) == ENCODER1ID) ||	\
		       ((ID) == ENCODER2ID))
int32_t MW_EncoderInit(encoderid_t id);

int32_t MW_GetEncoderVal(encoderid_t id);

#endif /* MIDLEWARE_INC_MW_ENCODER_H_ */
