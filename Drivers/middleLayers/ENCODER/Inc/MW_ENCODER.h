/*
 * MW_ENCODER.h
 *
 *  Created on: 2016/06/12
 *      Author: evaota
 */

#ifndef MIDLEWARE_INC_MW_ENCODER_H_
#define MIDLEWARE_INC_MW_ENCODER_H_

#include "stm32f1xx_hal.h"

typedef enum
{
	ENCODER1ID,
	ENCODER2ID
}encoderid_t;
/**Sample code(main)
 *
 *
 *
 *
 *
 */

/******************************* TIM ID *******************************/
#define IS_TIM_ID(ID) (((ID) == TIM1ID) || \
						 ((ID) == TIM2ID))

#endif /* MIDLEWARE_INC_MW_ENCODER_H_ */
