#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "message.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include "MW_flash.h"
#include "constManager.h"

#define _SCR_CURSOR_SET(x,y) MW_printf("\033[%d;%dH",(int)(y)+1,(int)(x)+1)
#define _SCR_CLEAR() MW_printf("\033[2J")

#define _NAME_LEN 15
#define _UNIT_LEN 5
#define _NAME_LEN_STR "15"
#define _UNIT_LEN_STR "5"

#define _MAX(x,y) ((x)>(y)?(x):(y))
#define _MIN(x,y) ((x)<(y)?(x):(y))

/*exclude dumy*/
#define _EDITLIST_NUM  ((sizeof(editlist)/sizeof(editlist[0]))-1)

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
  const_element_t dumy;/*may be destroy.*/
} adjust_t;

/*second, write const description.*/
static const adjust_t defaultad={
  .leftadjust = {
    .value = 0,
    .maxvalue = 20,
    .minvalue = -20,
    .display_name = "left adjust",
    .display_unit = "%",
  },
  .rightadjust = {
    .value = 0,
    .maxvalue = 20,
    .minvalue = -20,
    .display_name = "right adjust",
    .display_unit = "%"
  },
  .steeringtime = {
    .value = 50,
    .maxvalue = 200,
    .minvalue = 0,
    .display_name = "steering time",
    .display_unit = "*10ms",
  },
  .rctimeout = {
    .value = 10,
    .maxvalue = 999,
    .minvalue = 1,
    .display_name = "rc timeout",
    .display_unit = "s",
  },
  .rc_centre_thereshold = {
    .value = 4,
    .maxvalue = 6,
    .minvalue = 2,
    .display_name = "rc cntr theres",
    .display_unit = "/16",
  },

  /*template
  . = {
    .value = ,
    .maxvalue = ,
    .minvalue = ,
    .display_name = "",
    .display_unit = "",
  },
  */
  .dumy = {
    .value = 0,
    .maxvalue = 0,
    .minvalue = 0,
    .display_name = "N/A",
    .display_unit = "N/A",
  },
};

static 
adjust_t ad;

/*finary, add edit list.*/
static const_element_t *editlist[]={
  &(ad.rightadjust),
  &(ad.leftadjust),
  &(ad.steeringtime),
  &(ad.rctimeout),
  &(ad.rc_centre_thereshold),
};

/* reload default value */
static
void reloadDefault(void){
  ad = defaultad;
}

/*print a element*/
static
void ad_const_elementPrint(int i,const_element_t *list){
  MW_printf("[%2d] <%-"_NAME_LEN_STR"s> [%3d - %3d][%3d][%"_UNIT_LEN_STR"s]",
	 i,
	 list->display_name,
	 list->minvalue,
	 list->maxvalue,
	 list->value,
	 list->display_unit);
}

/*save data*/
static
int saveData(void){
  /*the boundary of size cause write failure, so this is arrenged the size.*/
  if(MW_flashWrite(&ad, WRITE_ADDR, sizeof(ad)&0xFFFFFFFE)==MW_FLASH_OK){
    return EXIT_SUCCESS;
  }
  else{
    return EXIT_FAILURE;
  }
}

/*print data*/
static
void adjustPrint(int point){
  int i;
  _SCR_CURSOR_SET(0, 0);
  MW_printf("  [NO] <%-"_NAME_LEN_STR"s> [MIN - MAX][Val][%"_UNIT_LEN_STR"s]\n","Name","Unit");
  MW_printf("-------------------------------------------------\n");
  for(i=0;i<(int)_EDITLIST_NUM+1;i++){
    if(i==point){
      MW_printf("> ");
    }else{
      MW_printf("  ");
    }
    if(i<(int)_EDITLIST_NUM){
      ad_const_elementPrint(i,editlist[i]);
      MW_printf("\n");
    }
    if(i==_EDITLIST_NUM+0){
      MW_printf("exit\n");
    }
  }
  flush();
  MW_printf("+---------------------+--------------------+\n");
  MW_printf("| %s...%*s|%s...%*s|\n","↑",15,"CURSOR UP","↓",15,"CURSOR DOWN");
  MW_printf("| %s...%*s|%s...%*s|\n","L1",15,"VALUE DEC@1","R1",15,"VALUE INC@1");
  MW_printf("| %s...%*s|%s...%*s|\n","L2",15,"VALUE DEC@10","R2",15,"VALUE INC@10");
  MW_printf("| %s...%*s|%s...%*s|\n","△",15,"LOAD DEFAULT","□",15,"SAVE DATA");
  MW_printf("+---------------------+--------------------+\n");
  flush();
}

/*do key task*/
static
int ad_keyTask(void){
  static int select;

  /*cursor motion*/
  {
    bool is_pressed_up;
    bool is_pressed_down;

    is_pressed_up   = __RC_ISPRESSED_UP(g_rc_data);
    is_pressed_down = __RC_ISPRESSED_DOWN(g_rc_data);
      
    if( is_pressed_up ||  is_pressed_down){
      _SCR_CURSOR_SET(0, select+2);
      MW_printf(" ");
      if( is_pressed_up ){
	select = _MAX(0, select-1);
      } else if( is_pressed_down ){
	select = _MIN((int)_EDITLIST_NUM, select+1);
      }
      _SCR_CURSOR_SET(0, select+2);
      MW_printf(">");
    }
  }
  
  /*cursor action*/
  {
    bool is_pressed_L1;
    bool is_pressed_L2;
    bool is_pressed_R1;
    bool is_pressed_R2;

    is_pressed_L1 = __RC_ISPRESSED_L1(g_rc_data);
    is_pressed_L2 = __RC_ISPRESSED_L2(g_rc_data);
    is_pressed_R1 = __RC_ISPRESSED_R1(g_rc_data);
    is_pressed_R2 = __RC_ISPRESSED_R2(g_rc_data);

    if(is_pressed_L1 || is_pressed_L2 ||
       is_pressed_R1 || is_pressed_R2){
      if(is_pressed_R1){
	editlist[select]->value = _MIN(editlist[select]->value+1, editlist[select]->maxvalue);
      }
      else if(is_pressed_R2){
	editlist[select]->value = _MIN(editlist[select]->value+10, editlist[select]->maxvalue);
      }
      else if(is_pressed_L1){
	editlist[select]->value = _MAX(editlist[select]->value-1, editlist[select]->maxvalue);
      }
      else if(is_pressed_L2){
	editlist[select]->value = _MAX(editlist[select]->value-10, editlist[select]->maxvalue);
      }

      _SCR_CURSOR_SET(2, select+2);
      ad_const_elementPrint(select, editlist[select]);
      _SCR_CURSOR_SET(1, select+2);
      MW_printf("*");
    }
  }

  _SCR_CURSOR_SET(0, _EDITLIST_NUM+8);
  
  /*reload value*/
  if(__RC_ISPRESSED_TRIANGLE(g_rc_data)){
    MW_printf("load default value");
    reloadDefault();    
  }

  /*save data*/
  if(__RC_ISPRESSED_SQARE(g_rc_data)){
    if(saveData()==EXIT_SUCCESS){
      MW_printf("save success");
      return 0;
    }
    else{
      MW_printf("save failure");
      return EXIT_FAILURE;
    }
  }

  /*screen update*/
  flush();

  return select==_EDITLIST_NUM?0:1;
}

static
int adjust(void){
  int count = 0;
  bool ret = true;
  bool had_key_pressed;
  _SCR_CLEAR();
  adjustPrint(0);

  while(ret){
    had_key_pressed = 
      __RC_ISPRESSED_UP(g_rc_data)||
      __RC_ISPRESSED_DOWN(g_rc_data)||
      __RC_ISPRESSED_L1(g_rc_data)||
      __RC_ISPRESSED_L2(g_rc_data)||
      __RC_ISPRESSED_R1(g_rc_data)||
      __RC_ISPRESSED_R2(g_rc_data)||
      __RC_ISPRESSED_TRIANGLE(g_rc_data)||
      __RC_ISPRESSED_SQARE(g_rc_data);
    
    if(had_key_pressed){
      if(count == 0||(count < 80&&count % 10==0)){
	ret = ad_keyTask();
      }
      count++;
    }
    else{
      count = 0;
    }
  }
  
  return ret == 1?EXIT_SUCCESS:EXIT_FAILURE;
}

int ad_main(void){
  ad = *(adjust_t*)WRITE_ADDR;
  return  adjust();
}
