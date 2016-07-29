#include "app.h"
#include "DD_Gene.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include <stdlib.h>
#include "message.h"
#include "MW_flash.h"

/*suspensionSystem*/
static
int suspensionSystem(void);
/*ABSystem*/
static 
int ABSystem(void);

/*メモ
 *g_ab_h...ABのハンドラ
 *g_md_h...MDのハンドラ
 *
 *g_rc_data...RCのデータ
 */

#define WRITE_ADDR (const void*)(0x8000000+0x400*(128-1))/*128[KiB]*/
flashError_t checkFlashWrite(void){
  const char data[]="HelloWorld!!TestDatas!!!\n"
    "however you like this microcomputer, you don`t be kind to this computer.";
  return MW_flashWrite(data,WRITE_ADDR,sizeof(data));
}

int appInit(void){
  message("msg","hell");

  switch(checkFlashWrite()){
  case MW_FLASH_OK:
    MW_printf("FLASH WRITE TEST SUCCESS\n");
    break;
  case MW_FLASH_LOCK_FAILURE:
    MW_printf("FLASH WRITE TEST LOCK FAILURE\n");
    break;
  case MW_FLASH_UNLOCK_FAILURE:
    MW_printf("FLASH WRITE TEST UNLOCK FAILURE\n");
    break;
  case MW_FLASH_ERASE_VERIFY_FAILURE:
    MW_printf("FLASH ERASE VERIFY FAILURE\n");
    break;
  case MW_FLASH_ERASE_FAILURE:
    MW_printf("FLASH ERASE FAILURE\n");
    break;
  case MW_FLASH_WRITE_VERIFY_FAILURE:
    MW_printf("FLASH WRITE TEST VERIFY FAILURE\n");
    break;
  case MW_FLASH_WRITE_FAILURE:
    MW_printf("FLASH WRITE TEST FAILURE\n");
    break;        
  default:
    MW_printf("FLASH WRITE TEST UNKNOWN FAILURE\n");
    break;
  }
    
    /*GPIO の設定などでMW,GPIOではHALを叩く*/
  return EXIT_SUCCESS;
}

/*application tasks*/
int appTask(void){
  int ret=0;

  /*それぞれの機構ごとに処理をする*/
  /*途中必ず定数回で終了すること。*/
  ret = suspensionSystem();
  if(ret){
    return ret;
  }

  ret = ABSystem();
  if(ret){
    return ret;
  }
  
  return EXIT_SUCCESS;
}

static 
int ABSystem(void){

  g_ab_h[0].dat = 0x00;
  if(__RC_ISPRESSED_CIRCLE(g_rc_data)){
    g_ab_h[0].dat |= AB0;
  }
  if(__RC_ISPRESSED_CROSS(g_rc_data)){
    g_ab_h[0].dat |= AB1;
  }

  return EXIT_SUCCESS;
}

/*プライベート 足回りシステム*/
static
int suspensionSystem(void){
  const int num_of_motor = 2;/*モータの個数*/
  int rc_analogdata;/*アナログデータ*/
  unsigned int idx;/*インデックス*/
  int i;

  /*for each motor*/
  for(i=0;i<num_of_motor;i++){
    /*それぞれの差分*/
    switch(i){
    case 0:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      idx = MECHA1_MD1;
      break;
    case 1:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      idx = MECHA1_MD2;
      break;
    default:return EXIT_FAILURE;
    }

    /*これは中央か?±3程度余裕を持つ必要がある。*/
    if(abs(rc_analogdata)<CENTRAL_THRESHOLD){
      g_md_h[idx].mode = D_MMOD_FREE;
      g_md_h[idx].duty = 0;
    }
    else{
      if(rc_analogdata > 0){
	/*前後の向き判定*/
	g_md_h[idx].mode = D_MMOD_FORWARD;
      }
      else{
	g_md_h[idx].mode = D_MMOD_BACKWARD;
      }
      /*絶対値を取りDutyに格納*/
      g_md_h[idx].duty = abs(rc_analogdata) * MD_GAIN;
    }
  }
  return EXIT_SUCCESS;
}
