/*
 * MW_IWDG.c
 *
 *  Created on: 2016/06/11
 *      Author: evaota
 */
#include "MW_IWDG.h"

static IWDG_HandleTypeDef hiwdg = {
  .Instance = IWDG,
  .Init.Prescaler = IWDG_PRESCALER_4,
  .Init.Reload = 4095
};

void MW_SetIWDGPrescaler(uint32_t Prescaler){
  hiwdg.Init.Prescaler = Prescaler;
}

void MW_SetIWDGReload(uint32_t Reload){
  hiwdg.Init.Reload = Reload;
}

/* IWDG init function */
int MW_IWDGInit(void){
  if( HAL_IWDG_Init(&hiwdg) != HAL_OK ){
    return EXIT_FAILURE;
  }
  HAL_IWDG_Start(&hiwdg);
  return EXIT_SUCCESS;
}

void MW_IWDGClr(void){
  HAL_IWDG_Refresh(&hiwdg);
}

uint32_t MW_IWDGStatus(void){
  return (uint32_t)HAL_IWDG_GetState(&hiwdg);
}

