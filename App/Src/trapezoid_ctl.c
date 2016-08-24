#include "DD_MD.h"
#include <stdlib.h>

#define _MIN2(x, y) (( x ) < ( y ) ? ( x ) : ( y ))

int control_trapezoid(const int rising_val,const int falling_val,DD_MDHand_t *g_md_h,int target_duty){

  int current_duty = g_md_h->duty;//現在のデューティ値
  int ctrl_val;//制御値

    /* 台形制御 */
    switch( g_md_h->mode ){
    case D_MMOD_FREE:
    case D_MMOD_BRAKE:
    case D_MMOD_FORWARD:
      if( current_duty < target_duty ){
        ctrl_val = current_duty + _MIN2(rising_val, target_duty - current_duty);
      }else if( current_duty > target_duty ){
        ctrl_val = current_duty - _MIN2(falling_val, current_duty - target_duty);
      }else{ ctrl_val = target_duty; }
      break;
    case D_MMOD_BACKWARD:
      current_duty = -current_duty;
      if( current_duty > target_duty ){
        ctrl_val = current_duty - _MIN2(rising_val, current_duty - target_duty);
      }else if( current_duty < target_duty ){
        ctrl_val = current_duty + _MIN2(falling_val, target_duty - current_duty);
      }else{ ctrl_val = target_duty; }
      break;
    default: return EXIT_FAILURE;
    }

    /*前後の向き判定*/
    if( ctrl_val > 0 ){
      g_md_h->mode = D_MMOD_FORWARD;
    }else if( ctrl_val < 0 ){
      g_md_h->mode = D_MMOD_BACKWARD;
    }else{
      g_md_h->mode = D_MMOD_BRAKE;
    }
    g_md_h->duty = abs(ctrl_val);

    return EXIT_SUCCESS;
}

