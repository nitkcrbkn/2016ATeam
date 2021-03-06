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


/* 駆動(大)の台形制御の変化量 */
tc_slope_lim_t tc_slope_lim_dri_lerge;

/* 駆動(小)の台形制御の変化量 */
tc_slope_lim_t tc_slope_lim_dri_small;

/* 橋展開の台形制御の変化量 */
tc_slope_lim_t tc_slope_lim_xpn;

/* 台形制御の変化量の初期化 */
static
void setTCVal(void);

/*suspensionSystem*/
static
int suspensionSystem(void);

/*bridgeSystem*/
static
int bridgeSystem(void);

/*メモ
 * *g_ab_h...ABのハンドラ
 * *g_md_h...MDのハンドラ
 *
 * _rc_data...RCのデータ
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

  if( __RC_ISPRESSED_R1(g_rc_data) && __RC_ISPRESSED_R2(g_rc_data) &&
      __RC_ISPRESSED_L1(g_rc_data) && __RC_ISPRESSED_L2(g_rc_data)){
    while( __RC_ISPRESSED_R1(g_rc_data) || __RC_ISPRESSED_R2(g_rc_data) ||
           __RC_ISPRESSED_L1(g_rc_data) || __RC_ISPRESSED_L2(g_rc_data)){
      SY_wait(10);
    }

    ad_main();
    setTCVal();
  }

  /*それぞれの機構ごとに処理をする*/
  /*途中必ず定数回で終了すること。*/
  ret = suspensionSystem();
  if( ret ){
    return ret;
  }

  ret = bridgeSystem();
  if( ret ){
    return ret;
  }

  return EXIT_SUCCESS;
}

/* 台形制御の変化量の初期化 */
static
void setTCVal(void){
  tc_slope_lim_dri_lerge.rising_val = g_adjust.tc_dril_rise.value;
  tc_slope_lim_dri_lerge.falling_val = g_adjust.tc_dril_fall.value;
  tc_slope_lim_dri_small.rising_val = g_adjust.tc_dris_rise.value;
  tc_slope_lim_dri_small.falling_val = g_adjust.tc_dris_fall.value;
  tc_slope_lim_xpn.rising_val = g_adjust.tc_xpn_rise.value;
  tc_slope_lim_xpn.falling_val = g_adjust.tc_xpn_fall.value;
}

/*プライベート 足回りシステム*/
static
int suspensionSystem(void){
  const int num_of_motor = 2; /*モータの個数*/
  int rc_analogdata; /*アナログデータ*/
  int is_reverse; /* 反転するか */
  unsigned int idx; /*インデックス*/
  unsigned int md_gain; /*アナログデータの補正値 */
  int ctl_motor_kind; /* 現在制御しているモータ */
  int target_val; /* 目標値 */
  const tc_slope_lim_t *tc_slope_lim_ptr; /* 台形制御の変化量のポインタ */

  /*for each motor*/
  for( ctl_motor_kind = ROB1_MTRL; ctl_motor_kind < num_of_motor; ctl_motor_kind++ ){
    is_reverse = 0;

    /*それぞれの差分*/
    switch( ctl_motor_kind ){
    case ROB1_MTRL:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_MTRL;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_MTRL;
      idx = ROB1_MTRL;
      tc_slope_lim_ptr = &tc_slope_lim_dri_lerge;
      break;
    case ROB1_DRIS:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIS;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIS;
      idx = ROB1_DRIS;
      tc_slope_lim_ptr = &tc_slope_lim_dri_small;
      break;
    default: return EXIT_FAILURE;
    }

    /*これは中央か?±3程度余裕を持つ必要がある。*/
    if( abs(rc_analogdata) < CENTRAL_THRESHOLD ){
      target_val = 0;
    }else{
      target_val = rc_analogdata * md_gain;
    }

    /*台数制御*/
    control_trapezoid(tc_slope_lim_ptr, &g_md_h[idx], target_val, is_reverse);
  }

  return EXIT_SUCCESS;
} /* suspensionSystem */

/*プライベート 橋展開システム*/
static
int bridgeSystem(void){
  if( __RC_ISPRESSED_CIRCLE(g_rc_data)){
    control_trapezoid(&tc_slope_lim_xpn, &g_md_h[ROB1_XPNS], -MD_MAX_DUTY_XPNS, _IS_REVERSE_XPNS);
  }else if( __RC_ISPRESSED_CROSS(g_rc_data)){
    control_trapezoid(&tc_slope_lim_xpn, &g_md_h[ROB1_XPNS], MD_MAX_DUTY_XPNS, _IS_REVERSE_XPNS);
  }else{
    control_trapezoid(&tc_slope_lim_xpn, &g_md_h[ROB1_XPNS], 0, _IS_REVERSE_XPNS);
  }

  return EXIT_SUCCESS;
} /* bridgeSystem */

