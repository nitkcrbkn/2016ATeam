#ifndef __CONST_MANAGER_H
#define __CONST_MANAGER_H

#define _RC_ADJUST_NUM 4

/*exclude dumy*/
#define _EDITLIST_NUM  9

#define WRITE_ADDR (const void*)(0x8000000+0x400*(128-1))/*128[KiB]*/

#define _NAME_LEN 15
#define _UNIT_LEN 5
#define _NAME_LEN_STR "15"
#define _UNIT_LEN_STR "5"


typedef struct {
  int value;
  int maxvalue;
  int minvalue;
  char display_name[_NAME_LEN+1];
  char display_unit[_UNIT_LEN+1];
} const_element_t;

/*first, add menber to this list.*/
typedef struct {
  const_element_t leftadjust;
  const_element_t rightadjust;
  const_element_t steeringtime;
  const_element_t rctimeout;
  const_element_t rc_centre_thereshold;
  const_element_t rc_centre_thereshold2;
  const_element_t rc_centre_thereshold3;
  const_element_t rc_centre_thereshold4;
  const_element_t rc_centre_thereshold5;
} adjust_t;

extern adjust_t g_adjust;
extern int g_c_data[_RC_ADJUST_NUM+_EDITLIST_NUM+4+1];

int ad_main(void);
void ad_init(void);

#endif
