#include "app.h"
#include "DD_Gene.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include <stdlib.h>
#include "MW_GPIO.h"
#include "MW_IWDG.h"
#include "message.h"
#include "trapezoid_ctl.h"
#include "MW_flash.h"
#include "constManager.h"

/* 駆動の台形制御の変化量 */
tc_slope_lim_t tc_slope_lim_dri;

/* アームの台形制御の変化量 */
tc_slope_lim_t tc_slope_lim_arm;

/* 台形制御の変化量の初期化 */
static
void setTCVal(void);

/*suspensionSystem*/
static
int suspensionSystem(void);

/*pushSystem*/
static
int armSystem(void);

/*メモ
 * *g_ab_h...ABのハンドラ
 * *g_md_h...MDのハンドラ
 *
 *  g_rc_data...RCのデータ
 */

int appInit(void){

  ad_init();

  setTCVal();
  /*GPIO の設定などでMW,GPIOではHALを叩く*/
  return EXIT_SUCCESS;
}

/*application tasks*/
int appTask(void){
  int ret = 0;

  if(__RC_ISPRESSED_R1(g_rc_data)&&__RC_ISPRESSED_R2(g_rc_data)&&
     __RC_ISPRESSED_L1(g_rc_data)&&__RC_ISPRESSED_L2(g_rc_data)){
    while(__RC_ISPRESSED_R1(g_rc_data)||__RC_ISPRESSED_R2(g_rc_data)||
	  __RC_ISPRESSED_L1(g_rc_data)||__RC_ISPRESSED_L2(g_rc_data))
      SY_wait(10);

    ad_main();
    setTCVal();
  }

  /*それぞれの機構ごとに処理をする*/
  /*途中必ず定数回で終了すること。*/
  ret = suspensionSystem();
  if( ret ){
    return ret;
  }

  ret = armSystem();
  if( ret ){
    return ret;
  }

  return EXIT_SUCCESS;
}

/* 台形制御の変化量の初期化 */
static
void setTCVal(void){
  tc_slope_lim_dri.rising_val = g_adjust.tc_dri_rise.value;
  tc_slope_lim_dri.falling_val = g_adjust.tc_dri_fall.value;
  tc_slope_lim_arm.rising_val = g_adjust.tc_arm_rise.value;
  tc_slope_lim_arm.falling_val = g_adjust.tc_arm_fall.value;
}

/* 足回りシステム*/
static
int suspensionSystem(void){
  const int num_of_motor = 4;       /*モータの個数*/
  int rc_analogdata;       /*アナログデータ*/
  int is_reverse;       /* 反転するか */
  unsigned int idx;       /*インデックス*/
  unsigned int md_gain;       /*アナログデータの補正値 */
  int ctl_motor_kind;       /* 現在制御しているモータ */
  int target_val;       /* 目標値 */

  /*for each motor*/
  for( ctl_motor_kind = ROB1_DRIL; ctl_motor_kind < num_of_motor; ctl_motor_kind++ ){
    is_reverse = 0;

    /*それぞれの差分*/
    switch( ctl_motor_kind ){
    case ROB1_DRIL:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIL;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIL;
      idx = ROB1_DRIL;
      break;
    case ROB1_DRIR:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIR;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIR;
      idx = ROB1_DRIR;
      break;
    case ROB1_DRIBL:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIBL;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIBL;
      idx = ROB1_DRIBL;
      break;
    case ROB1_DRIBR:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIBR;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIBR;
      idx = ROB1_DRIBR;
      break;
    default: return EXIT_FAILURE;
    }

    /* 目標値計算 */
    /*これは中央か?±3程度余裕を持つ必要がある。*/
    if( abs(rc_analogdata) < CENTRAL_THRESHOLD ){
      target_val = 0;
    }else{
      target_val = rc_analogdata * md_gain;
    }
    control_trapezoid(&tc_slope_lim_dri, &g_md_h[idx], target_val, is_reverse);
  }
  return EXIT_SUCCESS;
} /* suspensionSystem */

/*アームシステム*/
int armSystem(void){
  if( __RC_ISPRESSED_UP(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB1_ARM], MD_MAX_DUTY_ARM, _IS_REVERSE_ARM);
  }else if( __RC_ISPRESSED_DOWN(g_rc_data)){ /* アームのリミットスイッチが押されているか */
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB1_ARM], -MD_MAX_DUTY_ARM, _IS_REVERSE_ARM);
  }else{
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB1_ARM], 0, _IS_REVERSE_ARM);
  }

  return EXIT_SUCCESS;
} /* armSystem */
