/*
 * I2C.c
 *
 *  Created on: 2016/06/02
 *      Author: evaota
 */
#include "../Inc/MW_I2C.h"

static I2C_HandleTypeDef hi2c1 = {
  .Instance = I2C1,
  .Init.ClockSpeed = 100000,
  .Init.DutyCycle = I2C_DUTYCYCLE_2,
  .Init.OwnAddress1 = 0x00,
  .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
  .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
  .Init.OwnAddress2 = 0,
  .Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
  .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE,
};
static I2C_HandleTypeDef hi2c2 = {
  .Instance = I2C2,
  .Init.ClockSpeed = 100000,
  .Init.DutyCycle = I2C_DUTYCYCLE_2,
  .Init.OwnAddress1 = 0x00,
  .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
  .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
  .Init.OwnAddress2 = 0,
  .Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
  .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE,
};

static I2C_HandleTypeDef *i2cid[2] = {
  &hi2c1,
  &hi2c1
};

void MW_SetI2CClockSpeed(i2cid_t id,uint32_t ClockSpeed)
{
  assert_param(IS_I2C_ID(id));
  i2cid[(uint32_t)id]->Init.ClockSpeed = ClockSpeed;
}

void MW_SetI2CDutyCycle(i2cid_t id,uint32_t DutyCycle)
{
  assert_param(IS_I2C_ID(id));
  i2cid[(uint32_t)id]->Init.DutyCycle = DutyCycle;
}

void MW_SetI2COwnAddress1(i2cid_t id,uint32_t OwnAddress1)
{
  assert_param(IS_I2C_ID(id));
  i2cid[(uint32_t)id]->Init.OwnAddress1 = OwnAddress1;
}

void MW_SetI2COwnAddress2(i2cid_t id,uint32_t OwnAddress2)
{
  assert_param(IS_I2C_ID(id));
  i2cid[(uint32_t)id]->Init.OwnAddress2 = OwnAddress2;
}

void MW_SetI2CAddressingMode(i2cid_t id,uint32_t AddressingMode)
{
  assert_param(IS_I2C_ID(id));
  i2cid[(uint32_t)id]->Init.AddressingMode = AddressingMode;
}

void MW_SetI2CDualAddressMode(i2cid_t id,uint32_t DualAddressMode)
{
  assert_param(IS_I2C_ID(id));
  i2cid[(uint32_t)id]->Init.DualAddressMode = DualAddressMode;
}

void MW_SetI2CGeneralCallMode(i2cid_t id,uint32_t GeneralCallMode)
{
  assert_param(IS_I2C_ID(id));
  i2cid[(uint32_t)id]->Init.GeneralCallMode = GeneralCallMode;
}

void MW_SetI2CNoStretchMode(i2cid_t id,uint32_t NoStretchMode)
{
  assert_param(IS_I2C_ID(id));
  i2cid[(uint32_t)id]->Init.NoStretchMode = NoStretchMode;
}

/* I2C init function */
void MW_I2CInit(i2cid_t id)
{
  if (HAL_I2C_Init(i2cid[(uint32_t)id]) != HAL_OK){
    Error_Handler();
  }
}

int32_t MW_I2C1Transmit(uint8_t address,uint8_t *data,uint16_t size){
  if(HAL_I2C_Master_Transmit(&hi2c1,address,data,size,100)!=HAL_OK)
    return -1;
  return 0;
}

int32_t MW_I2C2Transmit(uint8_t address,uint8_t *data,uint16_t size){
  if(HAL_I2C_Master_Transmit(&hi2c2,address,data,size,100)!=HAL_OK)
    return -1;
  return 0;
}

int32_t MW_I2C1Receive(uint8_t address,uint8_t *data,uint16_t size)
{
  if(HAL_I2C_Master_Receive(&hi2c1,address,data,size,100)!=HAL_OK)
    return -1;
  return 0;}

int32_t MW_I2C2Receive(uint8_t address,uint8_t *data,uint16_t size)
{
  if(HAL_I2C_Master_Receive(&hi2c2,address,data,size,100)!=HAL_OK)
    return -1;
  return 0;
}

