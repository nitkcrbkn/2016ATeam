#include "app.h"
#include "DD_Gene.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include <stdlib.h>
#include "message.h"

/*suspensionSystem*/
static
int suspensionSystem(void);

/*メモ
 *g_ab_h...ABのハンドラ
 *g_md_h...MDのハンドラ
 *
 *g_rc_data...RCのデータ
 */

int appInit(void){
  message("msg","hell");
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
  
  return EXIT_SUCCESS;
}

/*プライベート 足回りシステム*/
static
int suspensionSystem(void){
  const int num_of_motor = 2;/*モータの個数*/
  int rc_analogdata;/*アナログデータ*/
  unsigned int idx;/*インデックス*/
  unsigned int md_gain;/*アナログデータの補正値 */
  int ctl_motor_kind;

  /*for each motor*/
  for(ctl_motor_kind=ROB1_DRIL; ctl_motor_kind<num_of_motor; ctl_motor_kind++){
    /*それぞれの差分*/
    switch(ctl_motor_kind){
    case ROB1_DRIL:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain=MD_GAIN_DRIL;
      /* 前後の向きを反転 */
      rc_analogdata *=-1;
      idx = ROB1_DRIL;
      break;
    case ROB1_DRIR:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain=MD_GAIN_DRIR;
      /* 前後の向きを反転 */
      rc_analogdata *=-1;
      idx = ROB1_DRIR;
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
      g_md_h[idx].duty = abs(rc_analogdata) * md_gain;
    }
  }
  return EXIT_SUCCESS;
}
