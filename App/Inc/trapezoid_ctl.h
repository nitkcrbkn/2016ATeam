#ifndef __TRAPEZOIDCTL_H
#define __TRAPEZOIDCTL_H

typedef struct {
  int rising_val; /* 立ち上がり値 */
  int falling_val; /* 立ち下がり値 */
} tc_slope_lim_t;

int control_trapezoid(const tc_slope_lim_t *inc_val,DD_MDHand_t *md_h,int target_val,int is_reverse);//台数制御

#endif






