#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "message.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include "MW_flash.h"
#include "constManager.h"

typedef enum{
  DM_HELP,
  DM_RC_VAL
} ad_disp_mode_t;

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
  .rc_centre_thereshold2 = {
    .value = 4,
    .maxvalue = 6,
    .minvalue = 2,
    .display_name = "rc cntr theres",
    .display_unit = "/16",
  },
  .rc_centre_thereshold3 = {
    .value = 4,
    .maxvalue = 6,
    .minvalue = 2,
    .display_name = "rc cntr theres",
    .display_unit = "/16",
  },
  .rc_centre_thereshold4 = {
    .value = 4,
    .maxvalue = 6,
    .minvalue = 2,
    .display_name = "rc cntr theres",
    .display_unit = "/16",
  },
  .rc_centre_thereshold5 = {
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
};

adjust_t g_adjust;

/*finary, add edit list.*/
const_element_t *editlist[_EDITLIST_NUM]={
  &(g_adjust.rightadjust),
  &(g_adjust.leftadjust),
  &(g_adjust.steeringtime),
  &(g_adjust.rctimeout),
  &(g_adjust.rc_centre_thereshold),
  &(g_adjust.rc_centre_thereshold2),
  &(g_adjust.rc_centre_thereshold3),
  &(g_adjust.rc_centre_thereshold4),
  &(g_adjust.rc_centre_thereshold5),
};

static
ad_disp_mode_t mode = DM_HELP;

int g_c_data[_RC_ADJUST_NUM+_EDITLIST_NUM+4+1];

static
int saveData(void);
static
int RC_adjust(void);

/* reload default value */
static
void reloadDefault(void){
  g_adjust = defaultad;
}

static
void load(void){
  int i;
  for(i=0;i<(int)_EDITLIST_NUM+4;i++){
    g_c_data[i] = ((int*)WRITE_ADDR)[i];
  }
  for(i=0;i<(int)_EDITLIST_NUM;i++){
    editlist[i]->value = g_c_data[i];
  }
}

static
int save(void){
  int i;
  for(i=0;i<(int)_EDITLIST_NUM;i++){
    g_c_data[i] = editlist[i]->value;
  }

  return saveData();  
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
  if(MW_flashWrite(g_c_data, WRITE_ADDR, sizeof(g_c_data)&0xFFFFFFFE)==MW_FLASH_OK){
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
  for(i=0;i<(int)_EDITLIST_NUM;i++){
    if(i==point){
      MW_printf("> ");
    }else{
      MW_printf("  ");
    }
    if(i<(int)_EDITLIST_NUM){
      ad_const_elementPrint(i,editlist[i]);
      MW_printf("\n");
    }
  }
  flush();

  _SCR_CURSOR_SET(0, _EDITLIST_NUM+3);
  switch(mode){
  case DM_HELP:
    MW_printf("+-----\033[1;33mHELP\033[0;39m---------------+------------------------+\n");
    MW_printf("| %s...%-18s| %s...%-18s|\n","↑ ","CURSOR UP","↓ ","CURSOR DOWN");
    MW_printf("| %s...%-18s| %s...%-18s|\n","L1","VALUE DEC@1","R1","VALUE INC@1");
    MW_printf("| %s...%-18s| %s...%-18s|\n","L2","VALUE DEC@10","R2","VALUE INC@10");
    MW_printf("| %s...%-18s| %s...%-18s|\n","× ","OK","○ ","sw HELP/RC data");
    MW_printf("| %s...%-18s| %s...%-18s|\n","△ ","LOAD DEF","□ ","SAVE 2 FLASH");
    MW_printf("+------------------------+------------------------+\n");
    flush();
    break;
  case DM_RC_VAL:
    MW_printf("------\033[1;33mRC VAL\033[0;39m--------------------------------------\n");
    DD_RCPrint(g_rc_data);
    flush();
    break;
  }
}

static
void interval_10ms(void){
  SY_wait(10);
}

static
void wait(unsigned int ms){
  SY_wait(ms);
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
	select = _MIN((int)_EDITLIST_NUM-1, select+1);
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
	editlist[select]->value = _MAX(editlist[select]->value-1, editlist[select]->minvalue);
      }
      else if(is_pressed_L2){
	editlist[select]->value = _MAX(editlist[select]->value-10, editlist[select]->minvalue);
      }

      _SCR_CURSOR_SET(2, select+2);
      ad_const_elementPrint(select, editlist[select]);
      _SCR_CURSOR_SET(1, select+2);
      MW_printf("*");
    }
  }

  _SCR_CURSOR_SET(0, _EDITLIST_NUM+10);
  
  /*reload value*/
  if(__RC_ISPRESSED_TRIANGLE(g_rc_data)){
    reloadDefault();
    message("msg","load default value");
    adjustPrint(select);
  }

  /*save data*/
  if(__RC_ISPRESSED_SQARE(g_rc_data)){
    if(save()==EXIT_SUCCESS){
      message("msg","save success");
      message("msg","plz reset");
      while(1);
      return 0;
    }
    else{
      message("err","save failure");
      message("msg","plz reset");
      while(1);
      return EXIT_FAILURE;
    }
  }

  else if(__RC_ISPRESSED_LEFT(g_rc_data)){
    message("msg","RC_ADJUST");
    wait(1000);      
    RC_adjust();
    adjustPrint(select);
    return 1;
  }
  
  /*screen update*/
  flush();

  if(__RC_ISPRESSED_CROSS(g_rc_data)){
    select = 0;
    message("msg","canceled");
    wait(1000);
    _SCR_CLEAR();
    _SCR_CURSOR_SET(0,0);
    return 0;
  }
  return 1;
}

static
int RC_adjust_Zero(void){
  _SCR_CLEAR();
  _SCR_CURSOR_SET(0, 10);
  MW_printf("adjust zero\n");
  MW_printf("○ ... adjust\n× ... cancel\n");
  flush();

  int p1x,p1y;
  int p2x,p2y;
  while(1){
    _SCR_CURSOR_SET(0, 0);
    MW_printf("(%d,%d),(%d,%d)\n",
	      __RC_GET_LX_VAL(g_rc_data),
	      __RC_GET_LY_VAL(g_rc_data),
	      __RC_GET_RX_VAL(g_rc_data),
	      __RC_GET_RY_VAL(g_rc_data)
	      );
    
    p1x = __RC_GET_LX_VAL(g_rc_data);
    p1y = __RC_GET_LY_VAL(g_rc_data);
    p2x = __RC_GET_RX_VAL(g_rc_data);
    p2y = __RC_GET_RY_VAL(g_rc_data);

    _SCR_CURSOR_SET(0,1);
    MW_printf("+---------------+  "" +---------------+  \n");
    MW_printf("|       |       |  "" |       |       |  \n");
    MW_printf("|       |       |  "" |       |       |  \n");
    MW_printf("|       |       |  "" |       |       |  \n");
    MW_printf("|-------+-------|  "" |-------+-------|  \n");
    MW_printf("|       |       |  "" |       |       |  \n");
    MW_printf("|       |       |  "" |       |       |  \n");
    MW_printf("|       |       |  "" |       |       |  \n");
    MW_printf("+---------------+  "" +---------------+  \n");
    
    _SCR_CURSOR_SET(((g_c_data[_EDITLIST_NUM + 0] - 16 + 3)*2)+2,g_c_data[_EDITLIST_NUM + 1] - 16+3+2);
    MW_printf("\033[41m  \033[49m");
    _SCR_CURSOR_SET(((g_c_data[_EDITLIST_NUM + 2] - 16 + 3)*2)+2+20,g_c_data[_EDITLIST_NUM + 3] - 16+3+2);
    MW_printf("\033[41m  \033[49m");
    {
      if(
	 p1x <= 16 + 3&&
	 p1x >= 16 - 3&&
	 p1y <= 16 + 3&&
	 p1y >= 16 - 3
	 ){

	_SCR_CURSOR_SET(((p1x - 16 + 3)*2)+2,p1y - 16+3+2);
	MW_printf("\033[42m  \033[49m");
      }
      if(
	 p2x <= 16 + 3&&
	 p2x >= 16 - 3&&
	 p2y <= 16 + 3&&
	 p2y >= 16 - 3
	 ){

	_SCR_CURSOR_SET(((p2x - 16 + 3)*2)+2+20,p2y - 16+3+2);
	MW_printf("\033[42m  \033[49m");
	flush();
	interval_10ms();
	interval_10ms();
	interval_10ms();
	interval_10ms();
	interval_10ms();
	interval_10ms();
	interval_10ms();
	interval_10ms();
	interval_10ms();
	interval_10ms();
      }
    }
    interval_10ms();

    if(__RC_ISPRESSED_CIRCLE(g_rc_data)){
      g_c_data[_EDITLIST_NUM+0] = __RC_GET_LX_VAL(g_rc_data);
      g_c_data[_EDITLIST_NUM+1] = __RC_GET_LY_VAL(g_rc_data);
      g_c_data[_EDITLIST_NUM+2] = __RC_GET_RX_VAL(g_rc_data);
      g_c_data[_EDITLIST_NUM+3] = __RC_GET_RY_VAL(g_rc_data);
      message("msg","adjusted");
      wait(1000);
      return EXIT_SUCCESS;
    }
    else if(__RC_ISPRESSED_CROSS(g_rc_data)){
      message("msg","canceled");
      wait(1000);
      _SCR_CLEAR();
      return EXIT_SUCCESS;
    }
  }
  
  return EXIT_SUCCESS;
}

static
int RC_adjust(void){
  if(RC_adjust_Zero()!=EXIT_SUCCESS)return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}

static
int adjust(void){
  int count = 0;
  bool ret = true;
  bool had_key_pressed;
  bool has_circle_pressed = false;
  
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
      __RC_ISPRESSED_SQARE(g_rc_data)||
      __RC_ISPRESSED_LEFT(g_rc_data)||
      __RC_ISPRESSED_CROSS(g_rc_data);

    if(had_key_pressed){
      if(count == 0||(count > 30&&count % 5==0)){
	ret = ad_keyTask();
      }
      count++;
    }
    else{
      count = 0;
    }

    if(!has_circle_pressed&&__RC_ISPRESSED_CIRCLE(g_rc_data)){
      if(mode==DM_HELP)mode = DM_RC_VAL;
      else mode = DM_HELP;
      _SCR_CLEAR();
      adjustPrint(0);
      has_circle_pressed = true;
    }
    if(!__RC_ISPRESSED_CIRCLE(g_rc_data)){
      has_circle_pressed = false;
    }

    if(mode == DM_RC_VAL){
      _SCR_CURSOR_SET(0, _EDITLIST_NUM + 3);
      MW_printf("------\033[1;33mRC VAL\033[0;39m--------------------------------------\n");
      DD_RCPrint(g_rc_data);
      flush();
    }
    
    interval_10ms();
  }
  
  return ret == 1?EXIT_SUCCESS:EXIT_FAILURE;
}


void ad_init(void){
  reloadDefault();
  load();
}

int ad_main(void){
  return  adjust();
}
