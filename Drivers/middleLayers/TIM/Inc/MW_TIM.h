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
void MW_TIMInit(timid_t id);

void MW_SetTIMPrescaler(timid_t id,uint32_t Prescaler);
void MW_SetTIMCounterMode(timid_t id,uint32_t CounterMode);
void MW_SetTIMPeriod(timid_t id,uint32_t Period);
void MW_SetTIMClockDivision(timid_t id,uint32_t ClockDivision);
void MW_SetTIMClockSource(timid_t id,uint32_t ClockSource);
void MW_SetTIMMasterOutputTrigger(timid_t id,uint32_t MasterOutputTrigger);
void MW_SetTIMMasterSlaveMode(timid_t id,uint32_t MasterSlaveMode);

void MW_TIMStartIT(timid_t id);
void MW_TIMStopIT(timid_t id);

uint32_t MW_GetTIMCounter(timid_t id);

void MW_TIM1Hadler(void);
void MW_TIM2Hadler(void);
void MW_ResetTIMxFlag(timid_t id);

#endif /* MIDLEWARE_INC_MW_TIM_H_ */
