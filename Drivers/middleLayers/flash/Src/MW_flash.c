/*
 *  Created on: 2016/07/29
 *      Author: oishi
 */
#include "MW_flash.h"
#include <stdlib.h>
#include <stdint.h>

static
flashError_t MW_flashUnlock(void);
static
flashError_t MW_flashLock(void);
static
flashError_t MW_flashElase(/*!erase here!*/ const void *flash_add, size_t size);

static
FLASH_EraseInitTypeDef elase_hand;

static
flashError_t MW_flashElase(/*!erase here!*/ const void *flash_add, size_t size){
  /*消去から。一ページ削除。周辺にコードを置かないようにリンカスクリプトを変更すること。*/
  {
    uint32_t page_err;
    elase_hand.Banks = FLASH_BANK_1;
    elase_hand.TypeErase = FLASH_TYPEERASE_PAGES;
    elase_hand.NbPages = 1;
    elase_hand.PageAddress = (uint32_t)flash_add;
    if( HAL_FLASHEx_Erase(&elase_hand, &page_err) != HAL_OK ){
      return MW_FLASH_ERASE_FAILURE;
    }
  }

  {
    uint32_t i;
    for( i = 0; i + 1 < size / 2; i++ ){
      if(((const uint16_t*)flash_add )[i] != 0xFFFF ){
        return MW_FLASH_ERASE_VERIFY_FAILURE;
      }
    }
  }
  return MW_FLASH_OK;
}

static
flashError_t MW_flashUnlock(void){
  if( HAL_FLASH_Unlock() != HAL_OK ){
    return MW_FLASH_UNLOCK_FAILURE;
  }
  return MW_FLASH_OK;
}

static
flashError_t MW_flashLock(void){
  if( HAL_FLASH_Lock() != HAL_OK ){
    return MW_FLASH_LOCK_FAILURE;
  }
  return MW_FLASH_OK;
}

flashError_t MW_flashWrite(const void *ptr, /*!write here!*/ const void *flash_add, size_t size){
  uint32_t address;
  flashError_t err;

  err = MW_flashUnlock();
  if( err != MW_FLASH_OK ){
    return err;
  }

  /*消去から。一ページ削除。周辺にコードを置かないようにリンカスクリプトを変更すること。*/
  err = MW_flashElase(flash_add, size);
  if( err != MW_FLASH_OK ){
    return err;
  }

  address = (uint32_t)flash_add;
  for(; address + 1 < (uint32_t)flash_add + size; address += 2 ,ptr += 2){
    if( HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, *(const uint16_t*)ptr) != HAL_OK ){
      return MW_FLASH_WRITE_FAILURE;
    }
    if( *(const uint16_t*)ptr != *(uint16_t*)address ){
      return MW_FLASH_WRITE_VERIFY_FAILURE;
    }
  }

  err = MW_flashLock();
  if( err != MW_FLASH_OK ){
    return err;
  }

  return EXIT_SUCCESS;
} /* MW_flashWrite */

