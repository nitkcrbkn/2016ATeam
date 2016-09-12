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

const tc_slope_lim_t tc_slope_lim_dri = {
  .rising_val = 200,
  .falling_val = 200,
};

const tc_slope_lim_t tc_slope_lim_xpn = {
  .rising_val = 400,
  .falling_val = 400,
};

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
      break;
    case ROB1_DRIS:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIS;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIS;
      idx = ROB1_DRIS;
      break;
    default: return EXIT_FAILURE;
    }

    /*これは中央か?±3程度余裕を持つ必要がある。*/
    if( abs(rc_analogdata) < CENTRAL_THRESHOLD ){
      target_val = 0;
    }else{
      target_val = rc_analogdata * md_gain;
    }
    
    if(idx == ROB1_MTRL &&
       (_IS_PRESSED_LIMITSW_MTRL() && target_val < 0)){
      g_md_h[ROB1_MTRL].mode = D_MMOD_BRAKE;
      g_md_h[ROB1_MTRL].duty = 0;
    }else if(idx == ROB1_DRIS &&
       (!_IS_PRESSED_LIMITSW_DRIS() && target_val < 0)){
      g_md_h[ROB1_DRIS].mode = D_MMOD_BRAKE;
      g_md_h[ROB1_DRIS].duty = 0;
    }else{
      /*台数制御*/
      control_trapezoid(&tc_slope_lim_dri, &g_md_h[idx], target_val, is_reverse);
    }
  }

  return EXIT_SUCCESS;
} /* suspensionSystem */

static
int bridgeSystem(void){
  if(__RC_ISPRESSED_CIRCLE(g_rc_data)){
    if(_IS_PRESSED_LIMITSW_BRIS()){
      g_md_h[ROB1_XPNS].mode = D_MMOD_BRAKE;
      g_md_h[ROB1_XPNS].duty = 0;      
    }else{
      control_trapezoid(&tc_slope_lim_xpn, &g_md_h[ROB1_XPNS], -MD_MAX_DUTY_XPNS, _IS_REVERSE_XPNS);
    }
  }else{
    control_trapezoid(&tc_slope_lim_xpn, &g_md_h[ROB1_XPNS], 0, _IS_REVERSE_XPNS);
  }
 
  return EXIT_SUCCESS;
}
