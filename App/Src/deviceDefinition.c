#include "app.h"
#include "DD_Gene.h"
#include "SystemTaskManager.h"

/*Address Definition*/
#if DD_NUM_OF_MD
/*MD Definition*/
DD_MDHand_t g_md_h[DD_NUM_OF_MD] = {
  { 0x10, /* address */
    0, /* default duty */
    D_MMOD_FREE, /* mode */
  },
  { 0x10, /* address */
    0, /* default duty */
    D_MMOD_FREE, /* mode */
  },
};
#endif
#if DD_NUM_OF_AB
/*AB Definition*/
DD_ABHand_t g_ab_h[DD_NUM_OF_AB] = {
  { 0x11, /* address */
    0x00, /* data */
  },
};
#endif

#if DD_NUM_OF_SV
DD_SV_t g_sv_h = {
  0x40,/*address*/
  {
    0,0,0,0
  }
};
#endif
