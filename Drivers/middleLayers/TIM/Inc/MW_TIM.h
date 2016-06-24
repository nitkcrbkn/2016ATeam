/*
 * TIM.h
 *
 *  Created on: 2016/06/05
 *      Author: evaota
 */

#ifndef MIDLEWARE_INC_MW_TIM_H_
#define MIDLEWARE_INC_MW_TIM_H_

#include "stm32f1xx_hal.h"

typedef enum
{
	TIM1ID,
	TIM2ID
}timid_t;

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

#define ENABLETIM1HADLE() {\
	__HAL_TIM_ENABLE_IT(timid[0], TIM_IT_UPDATE);\
	HAL_NVIC_SetPriority(TIM1_UP_IRQn, 0, 0);\
}
#define ENABLETIM2HADLE() {\
	__HAL_TIM_ENABLE_IT(timid[1], TIM_IT_UPDATE);\
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);\
}

#define DISABLETIM1HADLE() __HAL_TIM_DISABLE_IT(timid[0], TIM_IT_UPDATE)
#define DISABLETIM2HADLE() __HAL_TIM_DISABLE_IT(timid[1], TIM_IT_UPDATE)

void MW_TIM1Init(timid_t id);

void MW_SetTIMPrescaler(timid_t id,uint32_t Prescaler);
void MW_SetTIMCounterMode(timid_t id,uint32_t CounterMode);
void MW_SetTIMPeriod(timid_t id,uint32_t Period);
void MW_SetTIMClockDivision(timid_t id,uint32_t ClockDivision);
void MW_SetTIMClockSource(timid_t id,uint32_t ClockSource);
void MW_SetTIMMasterOutputTrigger(timid_t id,uint32_t MasterOutputTrigger);
void MW_SetTIMMasterSlaveMode(timid_t id,uint32_t MasterSlaveMode);

void MW_TIMStart(timid_t id);
void MW_TIMStop(timid_t id);

void MW_TIM1Hadler(void);
void MW_TIM2Hadler(void);
void MW_ResetTIMxFlag(timid_t id);

#endif /* MIDLEWARE_INC_MW_TIM_H_ */
