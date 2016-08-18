#include "app.h"
#include "DD_Gene.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include <stdlib.h>
#include "message.h"

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
  const int num_of_motor = 4; /*モータの個数*/
  const int rising_val = 200; /* 立ち上がり値 */
  const int falling_val = 200; /* 立ち下がり値 */
  int rc_analogdata; /*アナログデータ*/
  int ctrl_val; /* 制御値 */
  unsigned int idx; /*インデックス*/
  unsigned int md_gain; /*アナログデータの補正値 */
  int ctl_motor_kind; /* 現在制御しているモータ */
  int target_duty; /* 目標値 */
  int prev_duty; /* 現在の値 */

  /*for each motor*/
  for( ctl_motor_kind = ROB1_DRIL; ctl_motor_kind < num_of_motor; ctl_motor_kind++ ){
    /*それぞれの差分*/
    switch( ctl_motor_kind ){
    case ROB1_DRIL:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIL;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIL
      rc_analogdata = -rc_analogdata;
#endif
      idx = ROB1_DRIL;
      break;
    case ROB1_DRIR:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIR;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIL
      rc_analogdata = -rc_analogdata;
#endif
      idx = ROB1_DRIR;
      break;
    case ROB1_DRIBL:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIBL;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIBL
      rc_analogdata = -rc_analogdata;
#endif
      idx = ROB1_DRIBL;
      break;
    case ROB1_DRIBR:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIBR;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIBR
      rc_analogdata = -rc_analogdata;
#endif
      idx = ROB1_DRIBR;
      break;
    default: return EXIT_FAILURE;
    }

    /*これは中央か?±3程度余裕を持つ必要がある。*/
    if( abs(rc_analogdata) < CENTRAL_THRESHOLD ){
      target_duty = 0;
    }else{
      target_duty = rc_analogdata * md_gain;
    }
    prev_duty = g_md_h[idx].duty;

    /* 台形制御 */
    switch( g_md_h[idx].mode ){
    case D_MMOD_FREE:
    case D_MMOD_BRAKE:
    case D_MMOD_FORWARD:
      if( prev_duty < target_duty ){
        ctrl_val = prev_duty + min2(rising_val, target_duty - prev_duty);
      }else if( prev_duty > target_duty ){
        ctrl_val = prev_duty - min2(falling_val, prev_duty - target_duty);
      }else{ ctrl_val = target_duty; }
      break;
    case D_MMOD_BACKWARD:
      prev_duty *= -1;
      if( prev_duty > target_duty ){
        ctrl_val = prev_duty - min2(rising_val, prev_duty - target_duty);
      }else if( prev_duty < target_duty ){
        ctrl_val = prev_duty + min2(falling_val, target_duty - prev_duty);
      }else{ ctrl_val = target_duty; }
      break;
    default: return EXIT_FAILURE;
    }

    /*前後の向き判定*/
    if( ctrl_val > 0 ){
      g_md_h[idx].mode = D_MMOD_FORWARD;
    }else if( ctrl_val < 0 ){
      g_md_h[idx].mode = D_MMOD_BACKWARD;
    }else{
      g_md_h[idx].mode = D_MMOD_BRAKE;
    }
    g_md_h[idx].duty = abs(ctrl_val);
  }
  return EXIT_SUCCESS;
} /* suspensionSystem */

