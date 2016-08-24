
#define WRITE_ADDR (const void*)(0x8000000+0x400*(128-1))/*128[KiB]*/
flashError_t checkFlashWrite(void){
  const char data[]="HelloWorld!!TestDatas!!!\n"
    "however you like this microcomputer, you don`t be kind to this computer.\0\0";
  return MW_flashWrite(data,WRITE_ADDR,sizeof(data));
}

/*in check function,*/

  switch(checkFlashWrite()){
  case MW_FLASH_OK:
    message("msg","FLASH WRITE TEST SUCCESS\n%s",(const char*)WRITE_ADDR);
    break;
  case MW_FLASH_LOCK_FAILURE:
    message("err","FLASH WRITE TEST LOCK FAILURE\n");
    break;
  case MW_FLASH_UNLOCK_FAILURE:
    message("err","FLASH WRITE TEST UNLOCK FAILURE\n");
    break;
  case MW_FLASH_ERASE_VERIFY_FAILURE:
    message("err","FLASH ERASE VERIFY FAILURE\n");
    break;
  case MW_FLASH_ERASE_FAILURE:
    message("err","FLASH ERASE FAILURE\n");
    break;
  case MW_FLASH_WRITE_VERIFY_FAILURE:
    message("err","FLASH WRITE TEST VERIFY FAILURE\n");
    break;
  case MW_FLASH_WRITE_FAILURE:
    message("err","FLASH WRITE TEST FAILURE\n");
    break;
  default:
    message("err","FLASH WRITE TEST UNKNOWN FAILURE\n");
    break;
  }

