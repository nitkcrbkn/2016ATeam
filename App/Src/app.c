#include "DD_Gene.h"
#include "DD_RC.h"
#include "DD_RCDefinition.h"
#include "SystemTaskManager.h"
#include "app.h"
#include <stdlib.h>

/*足回り*/
static
int suspentionSystem(void);

/*メモ
 *g_ab_h...ABのハンドラ
 *g_md_h...MDのハンドラ
 *
 *g_rc_data...RCのデータ
 */

int appTask(void){
  int ret=0;

  ret = suspentionSystem();
  if(ret){
    return ret;
  }
  
  return EXIT_SUCCESS;
}

static
int suspentionSystem(void){
  const int num_of_motor = 2;
  int rc_analogdata;
  unsigned int idx;
  int i;

  /*for each motor*/
  for(i=0;i<num_of_motor;i++){
    /*diff*/
    switch(i){
    case 0: rc_analogdata = DD_RCGetRY(g_rc_data);idx = MECHA1_MD1;break;
    case 1: rc_analogdata = DD_RCGetLY(g_rc_data);idx = MECHA1_MD2;break;
    default:return EXIT_FAILURE;
    }

    /*is in centre?*/
    if(abs(rc_analogdata)<CENTRAL_THRESHOLD){
      g_md_h[idx].mode = D_MMOD_FREE;
      g_md_h[idx].duty = 0;
    }
    else{
      if(rc_analogdata > 0){
	g_md_h[idx].mode = D_MMOD_FORWARD;
      }
      else{
	g_md_h[idx].mode = D_MMOD_BACKWARD;
      }
      g_md_h[idx].duty = abs(rc_analogdata) * MD_GAIN;
    }
  }
  return EXIT_SUCCESS;
}


