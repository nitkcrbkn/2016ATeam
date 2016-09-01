/*
 * MW_ENCODER.c
 *
 *  Created on: 2016/06/12
 *      Author: evaota
 */
#include "MW_ENCODER.h"

static TIM_HandleTypeDef htim3 = {
  .Instance = TIM3,
  .Init = {
    .Prescaler = 0,
    .CounterMode = TIM_COUNTERMODE_UP,
    .Period = 0xFFFF,//counter max value is 0XFFFF
    .ClockDivision = TIM_CLOCKDIVISION_DIV1
  }
};

static TIM_HandleTypeDef htim4 = {
  .Instance = TIM4,
  .Init = {
    .Prescaler = 0,
    .CounterMode = TIM_COUNTERMODE_UP,
    .Period = 0xFFFF,//counter max value is 0XFFFF
    .ClockDivision = TIM_CLOCKDIVISION_DIV1
  }
};

static TIM_HandleTypeDef *encoderid[2] = {
  &htim3,
  &htim4
};


/* TIM3,4 init function */
int MW_EncoderInit(encoderid_t id)
{
  assert_param(IS_ENCODER_ID(id));
  
  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;

  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(encoderid[id], &sConfig) != HAL_OK)
    {
      return EXIT_FAILURE;
    }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(encoderid[id], &sMasterConfig) != HAL_OK)
    {
      return EXIT_FAILURE;
    }

  HAL_TIM_Base_Start(encoderid[id]);//enable counter
  
  return EXIT_SUCCESS;
  
}

int16_t MW_GetEncoderVal(encoderid_t id){
  assert_param(IS_ENCODER_ID(id));
  return __HAL_TIM_GET_COUNTER(encoderid[(uint32_t)id]);
}

void  MW_WriteEncoderVal(encoderid_t id,int16_t val){
  assert_param(IS_ENCODER_ID(id));
  __HAL_TIM_GET_COUNTER(encoderid[(uint32_t)id]) = val;
}
