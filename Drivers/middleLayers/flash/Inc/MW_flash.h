/*
 *  Created on: 2016/07/29
 *      Author: oishi
 */

#ifndef MIDLEWARE_FLASH_H_
#define MIDLEWARE_FLASH_H_

#include "stm32f1xx_hal.h"

typedef enum {
  MW_FLASH_OK=0,
  MW_FLASH_LOCK_FAILURE,
  MW_FLASH_UNLOCK_FAILURE,
  MW_FLASH_ERASE_VERIFY_FAILURE,
  MW_FLASH_ERASE_FAILURE,
  MW_FLASH_WRITE_VERIFY_FAILURE,
  MW_FLASH_WRITE_FAILURE,
} flashError_t;

/*erase and write.*/
/*erase 1 page.(0x400) DO NOT put program over one page*/
flashError_t MW_flashWrite(const void *ptr,/*!write here!*/const void *flash_add,size_t size);


#endif /* MIDLEWARE_FLASH_H_ */


















