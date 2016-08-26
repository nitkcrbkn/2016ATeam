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

/*suspensionSystem*/
static
int suspensionSystem(void);

/*pushSystem*/
static
int pushSystem(void);

/*ABSystem*/
static
int ABSystem(void);

/*メモ
 * *g_ab_h...ABのハンドラ
 * *g_md_h...MDのハンドラ
 *
 *  g_rc_data...RCのデータ
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
  ret = suspensionSystem();
  if( ret ){
    return ret;
  }

  ret = pushSystem();
  if( ret ){
    return ret;
  }
  
  ret = ABSystem();
  if( ret ){
    return ret;
  }

  return EXIT_SUCCESS;
}

/*プライベート 足回りシステム*/
static
int suspensionSystem(void){
  const int num_of_motor = 3;       /*モータの個数*/
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
#if _IS_REVERSE_DRIL
      is_reverse = 1;
#endif
      idx = ROB1_DRIL;
      break;
    case ROB1_DRIR:
      rc_analogdata = DD_RCGetRY(g_rc_data);
      md_gain = MD_GAIN_DRIR;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIR
      is_reverse = 1;
#endif
      idx = ROB1_DRIR;
      break;
    case ROB1_DRIB:
      rc_analogdata = DD_RCGetLY(g_rc_data);
      md_gain = MD_GAIN_DRIB;
      /* 前後の向きを反転 */
#if _IS_REVERSE_DRIB
      is_reverse = 1;
#endif
      idx = ROB1_DRIB;
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

    /* モータの回転を反転 */
    if( is_reverse ){
      target_val = -target_val;
    }

    control_trapezoid(&inc_val_dri , &g_md_h[idx], target_val);
  }
  return EXIT_SUCCESS;
} /* suspensionSystem */

int pushSystem(void){
  if( __RC_ISPRESSED_UP(g_rc_data)){
#if _REVERSE_PSH
    g_md_h[ROB1_PSH].mode = D_MMOD_FORWARD;
#else
    g_md_h[ROB1_PSH].mode = D_MMOD_BACKWARD;
#endif
    g_md_h[ROB1_PSH].duty = MD_MAX_DUTY_PSH;
  }else if( __RC_ISPRESSED_DOWN(g_rc_data)){
#if _REVERSE_PSH
    g_md_h[ROB1_PSH].mode = D_MMOD_BACKWARD;
#else
    g_md_h[ROB1_PSH].mode = D_MMOD_FORWARD;
#endif
    g_md_h[ROB1_PSH].duty = MD_MAX_DUTY_PSH;
  }else{
    g_md_h[ROB1_PSH].mode = D_MMOD_BRAKE;
    g_md_h[ROB1_PSH].duty = 0;
  }

  return EXIT_SUCCESS;
}

int ABSystem(void){
  static int has_pressed_R1;
  static int has_pressed_L1;

  if( __RC_ISPRESSED_R1(g_rc_data)){
    if( !has_pressed_R1 ){            /* R1が押され続けている間は処理を行わない */
      has_pressed_R1 = 1;
      if(( g_ab_h[ROB1_AB].dat & ( LIFTL | LIFTR )) != ( LIFTL | LIFTR )){           /*
                                                                                 *R1がすでに押されているか
                                                                                 **/
        g_ab_h[ROB1_AB].dat |= ( LIFTL | LIFTR );
      }else{
        g_ab_h[ROB1_AB].dat &= ~( LIFTL | LIFTR );
      }
    }
  }else{
    has_pressed_R1 = 0;
  }

  if( __RC_ISPRESSED_L1(g_rc_data)){
    if( !has_pressed_L1 ){            /* L1が押され続けている間は処理を行わない */
      has_pressed_L1 = 1;
      if(( g_ab_h[ROB1_AB].dat & ( PNCHL | PNCHR )) != ( PNCHL | PNCHR )){           /*
                                                                                 *L1がすでに押されているか
                                                                                 **/
        g_ab_h[ROB1_AB].dat |= ( PNCHL | PNCHR );
      }else{
        g_ab_h[ROB1_AB].dat &= ~( PNCHL | PNCHR );
      }
    }
  }else{
    has_pressed_L1 = 0;
  }

  return EXIT_SUCCESS;
} /* ABSystem */

