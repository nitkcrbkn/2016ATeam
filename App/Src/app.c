#include "app.h"
#include "DD_Gene.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include <stdlib.h>
#include "message.h"
#include "trapezoid_ctl.h"
#include "MW_flash.h"
#include "constManager.h"

const inc_val_t inc_val_dri = {
  .rising_val = 200,
  .falling_val = 200,
};

const inc_val_t inc_val_arm = {
  .rising_val = 200,
  .falling_val = 200,
};

/*suspensionSystem*/
/* static */
/* int suspensionSystem_modeA(void); */

static
int suspensionSystem_modeB(void);

/*armSystem*/
/* static */
/* int armSystem_modeA(void); */

static
int armSystem_modeB(void);

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

  if(__RC_ISPRESSED_R1(g_rc_data)&&__RC_ISPRESSED_R2(g_rc_data)&&
     __RC_ISPRESSED_L1(g_rc_data)&&__RC_ISPRESSED_L2(g_rc_data)){
    while(__RC_ISPRESSED_R1(g_rc_data)||__RC_ISPRESSED_R2(g_rc_data)||
	  __RC_ISPRESSED_L1(g_rc_data)||__RC_ISPRESSED_L2(g_rc_data));
    ad_main();
  }
  
  /*それぞれの機構ごとに処理をする*/
  /*途中必ず定数回で終了すること。*/
  ret = suspensionSystem_modeB();
  if( ret ){
    return ret;
  }

  ret = armSystem_modeB();
  if( ret ){
    return ret;
  }

  return EXIT_SUCCESS;
}

/*プライベート 足回りシステム*/
static
int suspensionSystem_modeB(void){
  const int num_of_motor = 3; /*モータの個数*/
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
#if _IS_REVERSE_DRIL
      is_reverse = 1;
#endif
      idx = ROB0_DRIL;
      break;
    case ROB0_DRIR:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIR;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIR
      is_reverse = 1;
#endif
      idx = ROB0_DRIR;
      break;
    case ROB0_DRIB:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIB;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIB
      is_reverse = 1;
#endif
      idx = ROB0_DRIB;
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

    /*モータの回転を反転 */
    if( is_reverse ){
      target_duty = -target_duty;
    }

    /* 台形制御 */
    control_trapezoid(&inc_val_dri, &g_md_h[idx], target_duty);
  }

  return EXIT_SUCCESS;
} /* suspensionSystem */

int armSystem_modeB(void){
  /* アーム基部の回転動作の制御 */
  if( __RC_ISPRESSED_L1(g_rc_data)){
#if _IS_REVERSE_ARMT
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMT], -MD_MAX_DUTY_ARMT);
#else
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMT], MD_MAX_DUTY_ARMT);
#endif
  }else if( __RC_ISPRESSED_R1(g_rc_data)){
#if _IS_REVERSE_ARMT
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMT], MD_MAX_DUTY_ARMT);
#else
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMT], -MD_MAX_DUTY_ARMT);
#endif
  }else{
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMT], 0);
  }

  /* アームの上下動作の制御 */
  if( __RC_ISPRESSED_UP(g_rc_data)){
#if _IS_REVERSE_ARME
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARME], -MD_MAX_DUTY_ARME);
#else
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARME], MD_MAX_DUTY_ARME);
#endif
  }else if( __RC_ISPRESSED_DOWN(g_rc_data)){
#if _IS_REVERSE_ARME
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARME], MD_MAX_DUTY_ARME);
#else
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARME], -MD_MAX_DUTY_ARME);
#endif
  }else{
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARME], 0);
  }

  /* アームの伸縮動作の制御 */
  if( __RC_ISPRESSED_LEFT(g_rc_data)){
#if _IS_REVERSE_ARMS
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMS], -MD_MAX_DUTY_ARMS);
#else
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMS], MD_MAX_DUTY_ARMS);
#endif
  }else if( __RC_ISPRESSED_RIGHT(g_rc_data)){
#if _IS_REVERSE_ARMS
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMS], -MD_MAX_DUTY_ARMS);
#else
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMS], MD_MAX_DUTY_ARMS);
#endif
  }else{
    control_trapezoid(&inc_val_arm , &g_md_h[ROB0_ARMS], 0);
  }

  return EXIT_SUCCESS;
}

