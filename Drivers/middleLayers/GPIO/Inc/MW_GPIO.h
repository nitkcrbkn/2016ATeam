/*
 * MW_GPIO.h
 *
 *  Created on: 2016/06/11
 *      Author: evaota
 */

#ifndef MIDLEWARE_SRC_MW_GPIO_H_
#define MIDLEWARE_SRC_MW_GPIO_H_

#include "stm32f1xx_hal.h"

typedef enum
{
	GPIOAID,
	GPIOBID,
	GPIOCID,
	GPIODID
}gpioid_t;
/**Sample code(main)
 *
 *
 *
 *
 *
 */

/******************************* TIM ID *******************************/
#define IS_GPIO_ID(ID) (((ID) == GPIOAID) || \
						 ((ID) == GPIOBID) || \
						 ((ID) == GPIOCID) || \
						 ((ID) == GPIODID))

#define ENABLECLKGPIOA() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENABLECLKGPIOB() __HAL_RCC_GPIOB_CLK_ENABLE()
#define ENABLECLKGPIOC() __HAL_RCC_GPIOC_CLK_ENABLE()
#define ENABLECLKGPIOD() __HAL_RCC_GPIOD_CLK_ENABLE()


void MW_SetGPIOPin(uint32_t Pin);
void MW_SetGPIOMode(uint32_t Mode);
void MW_SetGPIOPull(uint32_t Pull);
void MW_SetGPIOSpeed(uint32_t Speed);
void MW_GPIOInit(gpioid_t id);

void MW_GPIOWrite(gpioid_t id,uint16_t Pin,uint32_t PinState);
uint32_t MW_GPIORead(gpioid_t id,uint16_t Pin);
void MW_GPIOToggle(gpioid_t id,uint16_t Pin);

#endif /* MIDLEWARE_SRC_MW_GPIO_H_ */
