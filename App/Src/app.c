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

/* アーム回転と上下の台形制御の変化量 */
tc_slope_lim_t tc_slope_lim_arm;

/* アームの伸縮の台形制御の変化量 */
tc_slope_lim_t tc_slope_lim_armS;

/* 操作モード */
static ope_mode_t g_ope_mode = OPE_MODE_A;

/* スイッチを使うポートの初期化 */
static
void swInit(void);

/* 台形制御の変化量の初期化 */
static
void setTCVal(void);

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

  swInit();

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

  ret = changeOpeMode();
  if( ret ){
    return ret;
  }

  /* モードA,Bで関数を分ける */
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

/* スイッチを使うポートの初期化 */
static
void swInit(void){
  MW_SetGPIOPin(_LIMITSW_ARM_BACK_GPIOPIN);
  MW_SetGPIOMode(GPIO_MODE_INPUT);
  MW_SetGPIOPull(GPIO_PULLUP);
  MW_SetGPIOSpeed(GPIO_SPEED_FREQ_LOW);
  MW_GPIOInit(_LIMITSW_ARM_BACK_GPIOxID);
}

/* 台形制御の変化量の初期化 */
static
void setTCVal(void){
  tc_slope_lim_dri.rising_val = g_adjust.tc_dri_rise.value;
  tc_slope_lim_dri.falling_val = g_adjust.tc_dri_fall.value;
  tc_slope_lim_arm.rising_val = g_adjust.tc_arm_rise.value;
  tc_slope_lim_arm.falling_val = g_adjust.tc_arm_fall.value;
  tc_slope_lim_armS.rising_val = g_adjust.tc_armS_rise.value;
  tc_slope_lim_armS.falling_val = g_adjust.tc_armS_fall.value;
}

/* 操作モードの変更 */
static
int changeOpeMode(void){
  if( __RC_ISPRESSED_L2(g_rc_data)){
    g_ope_mode = OPE_MODE_B;
  }else if( __RC_ISPRESSED_R2(g_rc_data)){
    g_ope_mode = OPE_MODE_A;
  }

  return EXIT_SUCCESS;
}

/*足回りシステム モードA*/
static
int suspensionSystem_modeA(void){
  /* 目標値 */
  int DRIL_target_duty;
  int DRIR_target_duty;
  int DRIBF_target_duty;
  int DRIBB_target_duty;

  /* ボタンを１つ押すと、４つのモータが動作する */
  if( __RC_ISPRESSED_UP(g_rc_data)){
    DRIL_target_duty = -MD_MAX_DUTY_DRIL;
    DRIR_target_duty = -MD_MAX_DUTY_DRIR;
    DRIBF_target_duty = -MD_MAX_DUTY_DRIBF;
    DRIBB_target_duty = -MD_MAX_DUTY_DRIBB;
  }else if( __RC_ISPRESSED_DOWN(g_rc_data)){
    DRIL_target_duty = MD_MAX_DUTY_DRIL;
    DRIR_target_duty = MD_MAX_DUTY_DRIR;
    DRIBF_target_duty = MD_MAX_DUTY_DRIBF;
    DRIBB_target_duty = MD_MAX_DUTY_DRIBB;
  }else if( __RC_ISPRESSED_LEFT(g_rc_data)){
    DRIL_target_duty = MD_MAX_DUTY_DRIL;
    DRIR_target_duty = -MD_MAX_DUTY_DRIR;
    DRIBF_target_duty = 0;
    DRIBB_target_duty = 0;
  }else if( __RC_ISPRESSED_RIGHT(g_rc_data)){
    DRIL_target_duty = -MD_MAX_DUTY_DRIL;
    DRIR_target_duty = MD_MAX_DUTY_DRIR;
    DRIBF_target_duty = 0;
    DRIBB_target_duty = 0;
  }else{
    DRIL_target_duty = 0;
    DRIR_target_duty = 0;
    DRIBF_target_duty = 0;
    DRIBB_target_duty = 0;
  }

  control_trapezoid(&tc_slope_lim_dri, &g_md_h[ROB0_DRIL], DRIL_target_duty, _IS_REVERSE_DRIL);
  control_trapezoid(&tc_slope_lim_dri, &g_md_h[ROB0_DRIR], DRIR_target_duty, _IS_REVERSE_DRIR);
  control_trapezoid(&tc_slope_lim_dri, &g_md_h[ROB0_DRIBF], DRIBF_target_duty, _IS_REVERSE_DRIB);
  control_trapezoid(&tc_slope_lim_dri, &g_md_h[ROB0_DRIBB], DRIBB_target_duty, _IS_REVERSE_DRIB);

  return EXIT_SUCCESS;
} /* suspensionSystem_modeA */

/*足回りシステム モードB*/
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
      md_gain = MD_GAIN_DRIBF;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIB;
      idx = ROB0_DRIBF;
      break;
    case ROB0_DRIBB:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIBB;
      /* 前後の向きを反転 */
      is_reverse = _IS_REVERSE_DRIB;
      idx = ROB0_DRIBB;
      break;
    default: return EXIT_FAILURE;
    } /* switch */

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

/*アームシステム モードA*/
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

    /* アーム伸縮のリミットスイッチ */
    if( idx == ROB0_ARMS ){ /* ロボット伸縮の制御か */
      if( _IS_PRESSED_LIMITSW_ARM_BACK() && target_duty > 0 ){  /*
                                                                 *又は、アーム後端のリミットスイッチが押されているか
                                                                 **/
        g_md_h[ROB0_ARMS].mode = D_MMOD_BRAKE;
        g_md_h[ROB0_ARMS].duty = 0;
      }else{
        control_trapezoid(&tc_slope_lim_armS, &g_md_h[idx], target_duty, is_reverse);
      }
    }else{
      control_trapezoid(&tc_slope_lim_arm, &g_md_h[idx], target_duty, is_reverse);
    }
  }

  return EXIT_SUCCESS;
} /* armSystem_modeA */

/*足回りシステム モードB*/
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
    if( _IS_PRESSED_LIMITSW_ARM_BACK()){ /* アーム伸縮のリミットスイッチが押されているか */
      g_md_h[ROB0_ARMS].mode = D_MMOD_BRAKE;
      g_md_h[ROB0_ARMS].duty = 0;
    }else{
      control_trapezoid(&tc_slope_lim_armS, &g_md_h[ROB0_ARMS], MD_MAX_DUTY_ARMS, _IS_REVERSE_ARMS);
    }
  }else if( __RC_ISPRESSED_RIGHT(g_rc_data)){
    control_trapezoid(&tc_slope_lim_armS, &g_md_h[ROB0_ARMS], -MD_MAX_DUTY_ARMS, _IS_REVERSE_ARMS);
  }else{
    control_trapezoid(&tc_slope_lim_armS, &g_md_h[ROB0_ARMS], 0, _IS_REVERSE_ARMS);
  }

  return EXIT_SUCCESS;
} /* armSystem_modeB */

/*真空モータシステム*/
static
int vacSystem(void){
  static int has_pressed_cir;
  static int has_pressed_crs;
  static int has_pressed_sqr;
  static int has_pressed_tri;

  if( __RC_ISPRESSED_CIRCLE(g_rc_data)){
    /* ○が押され続けている間は処理を行わない */
    if( !has_pressed_cir ){
      has_pressed_cir = 1;
      g_ab_h[ROB0_VAC].dat ^= VAC0; /* トグル */
    }
  }else{
    has_pressed_cir = 0;
  }

  if( __RC_ISPRESSED_CROSS(g_rc_data)){
    /* ×が押され続けている間は処理を行わない */
    if( !has_pressed_crs ){
      has_pressed_crs = 1;
      g_ab_h[ROB0_VAC].dat ^= VAC1; /* トグル */
    }
  }else{
    has_pressed_crs = 0;
  }

  if( __RC_ISPRESSED_SQARE(g_rc_data)){
    /* □が押され続けている間は処理を行わない */
    if( !has_pressed_sqr ){
      has_pressed_sqr = 1;
      g_ab_h[ROB0_VAC].dat ^= VAC2; /* トグル */
    }
  }else{
    has_pressed_sqr = 0;
  }

  if( __RC_ISPRESSED_TRIANGLE(g_rc_data)){
    /* △が押され続けている間は処理を行わない */
    if( !has_pressed_tri ){
      has_pressed_tri = 1;
      g_ab_h[ROB0_VAC].dat ^= VAC3; /* トグル */
    }
  }else{
    has_pressed_tri = 0;
  }

  return EXIT_SUCCESS;
} /* vacSystem */

/*LEDシステム*/
static int LEDSystem(void){
  switch( g_ope_mode ){
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
} /* LEDSystem */

