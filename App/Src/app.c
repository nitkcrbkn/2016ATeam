#include "app.h"
#include "DD_Gene.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include <stdlib.h>
#include "message.h"
#include "trapezoid_ctl.h"
#include "MW_flash.h"
#include "constManager.h"

const tc_slope_lim_t tc_slope_lim_dri = {
  .rising_val = 200,
  .falling_val = 200,
};

const tc_slope_lim_t tc_slope_lim_arm = {
  .rising_val = 200,
  .falling_val = 400,
};

/* 操作モード */
static ope_mode_t g_ope_mode = OPE_MODE_A;

/* 操作モード変更 */
static
int changeOpeMode(void);

/*suspensionSystem*/
static
int suspensionSystem_modeA(void);

static
int suspensionSystem_modeB(void);

/*armSystem*/
static
int armSystem_modeA(void);

static
int armSystem_modeB(void);

/*vacuumSystem*/
static
int vacSystem(void);

/*LEDSystem*/
static
int LEDSystem(void);

/*メモ
 * *g_ab_h...ABのハンドラ
 * *g_md_h...MDのハンドラ
 *
 * *g_rc_data...RCのデータ
 */

int appInit(void){
  ad_init();

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
    }
    ad_main();
  }

  /*それぞれの機構ごとに処理をする*/
  /*途中必ず定数回で終了すること。*/

  ret = changeOpeMode();
  if( ret ){
    return ret;
  }

  /* モードA,Bで関数を分けるb */
  switch( g_ope_mode ){
  case OPE_MODE_A:
    ret = suspensionSystem_modeA();
    if( ret ){
      return ret;
    }

    ret = armSystem_modeA();
    if( ret ){
      return ret;
    }
    break;

  case OPE_MODE_B:
    ret = suspensionSystem_modeB();
    if( ret ){
      return ret;
    }

    ret = armSystem_modeB();
    if( ret ){
      return ret;
    }
    break;

  default: return EXIT_FAILURE;
  }

  ret = vacSystem();
  if( ret ){
    return ret;
  }

  ret = LEDSystem();
  if( ret ){
    return ret;
  }

  return EXIT_SUCCESS;
} /* appTask */

static
int changeOpeMode(void){
  if( __RC_ISPRESSED_L2(g_rc_data)){
    g_ope_mode = OPE_MODE_B;
  }else if( __RC_ISPRESSED_R2(g_rc_data)){
    g_ope_mode = OPE_MODE_A;
  }

  return EXIT_SUCCESS;
}

/*プライベート 足回りシステム*/
static
int suspensionSystem_modeA(void){
  /* ボタンを１つ押すと、４つのモータが動作する */
  if( __RC_ISPRESSED_UP(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIL], -MD_MAX_DUTY_DRIL, _IS_REVERSE_DRIL);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIR], -MD_MAX_DUTY_DRIR, _IS_REVERSE_DRIR);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBF], -MD_MAX_DUTY_DRIB, _IS_REVERSE_DRIB);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBB], -MD_MAX_DUTY_DRIB, _IS_REVERSE_DRIB);
  }else if( __RC_ISPRESSED_DOWN(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIL], MD_MAX_DUTY_DRIL, _IS_REVERSE_DRIL);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIR], MD_MAX_DUTY_DRIR, _IS_REVERSE_DRIR);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBF], MD_MAX_DUTY_DRIB, _IS_REVERSE_DRIB);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBB], MD_MAX_DUTY_DRIB, _IS_REVERSE_DRIB);
  }else if( __RC_ISPRESSED_LEFT(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIL], MD_MAX_DUTY_DRIL, _IS_REVERSE_DRIL);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIR], -MD_MAX_DUTY_DRIR, _IS_REVERSE_DRIR);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBF], 0, _IS_REVERSE_DRIB);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBB], 0, _IS_REVERSE_DRIB);
  }else if( __RC_ISPRESSED_RIGHT(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIL], -MD_MAX_DUTY_DRIL, _IS_REVERSE_DRIL);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIR], MD_MAX_DUTY_DRIR, _IS_REVERSE_DRIR);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBF], 0, _IS_REVERSE_DRIB);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBB], 0, _IS_REVERSE_DRIB);
  }else{
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIL], 0, _IS_REVERSE_DRIL);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIR], 0, _IS_REVERSE_DRIR);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBF], 0, _IS_REVERSE_DRIB);
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_DRIBB], 0, _IS_REVERSE_DRIB);
  }

  return EXIT_SUCCESS;
} /* suspensionSystem_modeA */

static
int suspensionSystem_modeB(void){
  const int num_of_motor = 4; /*モータの個数*/
  int rc_analogdata; /*アナログデータ*/
  int is_reverse; /* 反転するか */
  unsigned int idx; /*インデックス*/
  unsigned int md_gain; /*アナログデータの補正値 */
  int ctl_motor_kind; /* 現在制御しているモータ */
  int target_duty; /* 目標値 */

  /*for each motor*/
  for( ctl_motor_kind = ROB0_DRIL; ctl_motor_kind < num_of_motor; ctl_motor_kind++ ){
    is_reverse = 0;

    /*それぞれの差分*/
    switch( ctl_motor_kind ){
    case ROB0_DRIL:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIL;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIL;
      idx = ROB0_DRIL;
      break;
    case ROB0_DRIR:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIR;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIR;
      idx = ROB0_DRIR;
      break;
    case ROB0_DRIBF:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIB;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIB;
      idx = ROB0_DRIBF;
      break;
    case ROB0_DRIBB:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIB;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIB;
      idx = ROB0_DRIBB;
      break;
    default: return EXIT_FAILURE;
    }

    /* 目標値を計算 */
    /*これは中央か?±3程度余裕を持つ必要がある。*/
    if( abs(rc_analogdata) < CENTRAL_THRESHOLD ){
      target_duty = 0;
    }else{
      target_duty = rc_analogdata * md_gain;
    }

    /* 台形制御 */
    control_trapezoid(&tc_slope_lim_dri, &g_md_h[idx], target_duty, is_reverse);
  }

  return EXIT_SUCCESS;
} /* suspensionSystem_modeB */

static
int armSystem_modeA(void){
  const int num_of_motor = ROB0_ARMT + 3; /*モータの個数*/
  int rc_analogdata; /*アナログデータ*/
  int is_reverse; /* 反転するか */
  unsigned int idx; /*インデックス*/
  unsigned int md_gain; /*アナログデータの補正値 */
  int ctl_motor_kind; /* 現在制御しているモータ */
  int target_duty; /* 目標値 */

  /*for each motor*/
  for( ctl_motor_kind = ROB0_ARMT; ctl_motor_kind < num_of_motor; ctl_motor_kind++ ){
    is_reverse = 0;

    /*それぞれの差分*/
    switch( ctl_motor_kind ){
    case ROB0_ARMT:
      rc_analogdata = DD_RCGetLX(g_rc_data);
      md_gain = MD_GAIN_ARMT;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_ARMT;
      idx = ROB0_ARMT;
      break;
    case ROB0_ARME:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_ARME;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_ARME;
      idx = ROB0_ARME;
      break;
    case ROB0_ARMS:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_ARMS;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_ARMS;
      idx = ROB0_ARMS;
      break;
    default: return EXIT_FAILURE;
    }

    /* 目標値を計算 */
    /*これは中央か?±3程度余裕を持つ必要がある。*/
    if( abs(rc_analogdata) < CENTRAL_THRESHOLD ){
      target_duty = 0;
    }else{
      target_duty = rc_analogdata * md_gain;
    }

    /* 台形制御 */
    control_trapezoid(&tc_slope_lim_dri, &g_md_h[idx], target_duty, is_reverse);
  }

  return EXIT_SUCCESS;
} /* armSystem_modeA */

static
int armSystem_modeB(void){
  /* アーム基部の回転動作の制御 */
  if( __RC_ISPRESSED_L1(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_ARMT], -MD_MAX_DUTY_ARMT, _IS_REVERSE_ARMT);
  }else if( __RC_ISPRESSED_R1(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_ARMT], MD_MAX_DUTY_ARMT, _IS_REVERSE_ARMT);
  }else{
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_ARMT], 0, _IS_REVERSE_ARMT);
  } 

  /* アームの上下動作の制御 */
  if( __RC_ISPRESSED_UP(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_ARME], -MD_MAX_DUTY_ARME, _IS_REVERSE_ARME);
  }else if( __RC_ISPRESSED_DOWN(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_ARME], MD_MAX_DUTY_ARME, _IS_REVERSE_ARME);
  }else{
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_ARME], 0, _IS_REVERSE_ARME);
  }

  /* アームの伸縮動作の制御 */
  if( __RC_ISPRESSED_LEFT(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_ARMS], MD_MAX_DUTY_ARMS, _IS_REVERSE_ARMS);
  }else if( __RC_ISPRESSED_RIGHT(g_rc_data)){
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_ARMS], -MD_MAX_DUTY_ARMS, _IS_REVERSE_ARMS);
  }else{
    control_trapezoid(&tc_slope_lim_arm, &g_md_h[ROB0_ARMS], 0, _IS_REVERSE_ARMS);
  }

  return EXIT_SUCCESS;
} /* armSystem_modeB */

static
int vacSystem(void){
  static int has_pressed_tri;

  if( __RC_ISPRESSED_TRIANGLE(g_rc_data)){
    /* △が押され続けている間は処理を行わない */
    if( !has_pressed_tri ){            
      has_pressed_tri = 1;
      /* △がすでに押されているか */
      if(( g_ab_h[ROB0_VAC].dat & ( VAC0 | VAC1 | VAC2 | VAC3 )) != ( VAC0 | VAC1 | VAC2 | VAC3 )){
        g_ab_h[ROB0_VAC].dat |= ( VAC0 | VAC1 | VAC2 | VAC3 );
      }else{
        g_ab_h[ROB0_VAC].dat &= ~( VAC0 | VAC1 | VAC2 | VAC3 );
      }
    }
  }else{
    has_pressed_tri = 0;
  }
 
  return EXIT_SUCCESS;
}

static int LEDSystem(void){
  
  switch(g_ope_mode){
  case OPE_MODE_A:
    g_led_mode = lmode_1;
    break;
  case OPE_MODE_B:
    g_led_mode = lmode_3;
    break;
  default:
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

}

