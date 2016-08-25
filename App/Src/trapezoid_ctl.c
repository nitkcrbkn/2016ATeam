#include "DD_MD.h"
#include "SystemTaskManager.h"
#include <stdlib.h>

int control_trapezoid(int rising_val, int falling_val,DD_MDHand_t *md_h,int target_val){

  int current_val = md_h->duty; /* 現在のデューティ値 */
  int ctrl_val; /* 制御値 */

  /* 台形制御 */
  switch( md_h->mode ){
  case D_MMOD_FREE:
  case D_MMOD_BRAKE:
  case D_MMOD_FORWARD:
    if( current_val < target_val ){
      ctrl_val = current_val + _MIN(rising_val, target_val - current_val);
    }else if( current_val > target_val ){
      ctrl_val = current_val - _MIN(falling_val, current_val - target_val);
    }else{ ctrl_val = target_val; }
    break;
  case D_MMOD_BACKWARD:
    current_val = -current_val;
    if( current_val > target_val ){
      ctrl_val = current_val - _MIN(rising_val, current_val - target_val);
    }else if( current_val < target_val ){
      ctrl_val = current_val + _MIN(falling_val, target_val - current_val);
    }else{ ctrl_val = target_val; }
    break;
  default: return EXIT_FAILURE;
  }

  /*前後の向き判定*/
  if( ctrl_val > 0 ){
    md_h->mode = D_MMOD_FORWARD;
  }else if( ctrl_val < 0 ){
    md_h->mode = D_MMOD_BACKWARD;
  }else{
    md_h->mode = D_MMOD_BRAKE;
  }
  md_h->duty = abs(ctrl_val);

  return EXIT_SUCCESS;
} /* control_trapezoid */

