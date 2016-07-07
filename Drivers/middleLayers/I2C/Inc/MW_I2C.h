/*
 * I2C.h
 *
 *  Created on: 2016/06/02
 *      Author: evaota
 */

#ifndef MIDLEWARE_INC_MW_I2C_H_
#define MIDLEWARE_INC_MW_I2C_H_

#include "stm32f1xx_hal.h"

typedef enum
  {
    STANDARD,
    HIGH
  }speed_t;

typedef enum
  {
    I2C1ID = 0,
    I2C2ID = 1,
  }i2cid_t;

/**Saple code(main)
 *
 *
 *
 *
 *
 */

/******************************* USART ID *******************************/
#define IS_I2C_ID(ID) (((ID) == I2C1ID) ||	\
		       ((ID) == I2C2ID))
/**I2C1 GPIO Configuration
   PB8     ------> I2C1_SCL
   PB9     ------> I2C1_SDA
*/

/**I2C2 GPIO Configuration
   PB10     ------> I2C2_SCL
   PB11     ------> I2C2_SDA
*/
void MW_I2CInit(i2cid_t id);

void MW_SetI2CClockSpeed(i2cid_t id,uint32_t ClockSpeed);
void MW_SetI2CDutyCycle(i2cid_t id,uint32_t DutyCycle);
void MW_SetI2COwnAddress1(i2cid_t id,uint32_t OwnAddress1);
void MW_SetI2COwnAddress2(i2cid_t id,uint32_t OwnAddress2);
void MW_SetI2CAddressingMode(i2cid_t id,uint32_t AddressingMode);
void MW_SetI2CDualAddressMode(i2cid_t id,uint32_t DualAddressMode);
void MW_SetI2CGeneralCallMode(i2cid_t id,uint32_t GeneralCallMode);
void MW_SetI2CNoStretchMode(i2cid_t id,uint32_t NoStretchMode);

int32_t MW_I2C1Transmit(uint8_t address,const uint8_t *data,uint16_t size);
int32_t MW_I2C2Transmit(uint8_t address,const uint8_t *data,uint16_t size);
int32_t MW_I2C1Receive(uint8_t address,uint8_t *data,uint16_t size);
int32_t MW_I2C2Receive(uint8_t address,uint8_t *data,uint16_t size);

#endif /* MIDLEWARE_INC_MW_I2C_H_ */
