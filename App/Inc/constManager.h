#ifndef __CONST_MANAGER_H

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
} adjust_t;

extern adjust_t g_adjust;

int ad_main(void);
void ad_init(void);

#endif
