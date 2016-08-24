#include "app.h"
#include "DD_Gene.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include <stdlib.h>
#include "message.h"
#include "trapezoid_ctl.h"

#define min2(x, y) (( x ) < ( y ) ? ( x ) : ( y ))

/*suspensionSystem*/
static
int suspensionSystem(void);

/*メモ
 * *g_ab_h...ABのハンドラ
 * *g_md_h...MDのハンドラ
 *
 **g_rc_data...RCのデータ
 */

int appInit(void){
  message("msg", "hell");
  /*GPIO の設定などでMW,GPIOではHALを叩く*/
  return EXIT_SUCCESS;
}

/*application tasks*/
int appTask(void){
  int ret = 0;

  /*それぞれの機構ごとに処理をする*/
  /*途中必ず定数回で終了すること。*/
  ret = suspensionSystem();
  if( ret ){
    return ret;
  }

  return EXIT_SUCCESS;
}

/*プライベート 足回りシステム*/
static
int suspensionSystem(void){
  const int num_of_motor = 2; /*モータの個数*/
  const int rising_val = 200; /* 立ち上がり値 */
  const int falling_val = 200; /* 立ち下がり値 */
  int rc_analogdata; /*アナログデータ*/
  int reverse_flg; /* 反転するか */
  unsigned int idx; /*インデックス*/
  unsigned int md_gain; /*アナログデータの補正値 */
  int ctl_motor_kind; /* 現在制御しているモータ */
  int target_duty; /* 目標値 */

  /*for each motor*/
  for( ctl_motor_kind = ROB1_DRIL; ctl_motor_kind < num_of_motor; ctl_motor_kind++ ){
    reverse_flg = 0;

    /*それぞれの差分*/
    switch( ctl_motor_kind ){
    case ROB1_DRIL:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIL;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIL
      reverse_flg = 1;
#endif
      idx = ROB1_DRIL;
      break;
    case ROB1_DRIR:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIR;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIL
      reverse_flg = 1;
#endif
      idx = ROB1_DRIR;
      break;
    default: return EXIT_FAILURE;
    }

    /*これは中央か?±3程度余裕を持つ必要がある。*/
    if( abs(rc_analogdata) < CENTRAL_THRESHOLD ){
      target_duty = 0;
    }else{
      target_duty = rc_analogdata * md_gain;
    }

    if(reverse_flg) target_duty = -target_duty;

    /*台数制御*/
    control_trapezoid(rising_val, falling_val ,&g_md_h[idx] ,target_duty);

  }
  return EXIT_SUCCESS;
} /* suspensionSystem */

