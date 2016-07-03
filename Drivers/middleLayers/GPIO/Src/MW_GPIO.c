/*
 * MW_GPIO.c
 *
 *  Created on: 2016/06/11
 *      Author: evaota
 */

#include "../Inc/MW_GPIO.h"

static GPIO_InitTypeDef GPIO_InitStruct;
static GPIO_TypeDef *gpioid[4] = {
  GPIOA,
  GPIOB,
  GPIOC,
  GPIOD
};

void MW_SetGPIOPin(uint32_t Pin){
  GPIO_InitStruct.Pin = Pin;
}

void MW_SetGPIOMode(uint32_t Mode){
  GPIO_InitStruct.Mode = Mode;
}

void MW_SetGPIOPull(uint32_t Pull){
  GPIO_InitStruct.Pull = Pull;
}

void MW_SetGPIOSpeed(uint32_t Speed){
  GPIO_InitStruct.Speed = Speed;
}

void MW_GPIOInit(gpioid_t id){
  assert_param(IS_GPIO_ID(id));
  HAL_GPIO_Init(gpioid[(uint32_t)id], &GPIO_InitStruct);
}

void MW_GPIOWrite(gpioid_t id, uint16_t Pin, uint32_t PinState){
  assert_param(IS_GPIO_ID(id));
  HAL_GPIO_WritePin(gpioid[(uint32_t)id], Pin, (GPIO_PinState)PinState);
}

uint32_t MW_GPIORead(gpioid_t id, uint16_t Pin){
  assert_param(IS_GPIO_ID(id));
  return (uint32_t)HAL_GPIO_ReadPin(gpioid[(uint32_t)id], Pin);
}

void MW_GPIOToggle(gpioid_t id, uint16_t Pin){
  assert_param(IS_GPIO_ID(id));
  HAL_GPIO_TogglePin(gpioid[(uint32_t)id], Pin);
}

