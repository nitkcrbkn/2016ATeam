int main(void)
{
  DD_SV_t test = {
    .i2cadd = 0x40,/*address*/
    .val = {
      0,0,0,0
    }
  };
  SV_Init(&test);
        
  while(1)
    {

      if(__RC_ISPRESSED_CIRCLE(g_rc_data)){
	g_sv_h.val[0] = 350;
	g_sv_h.val[1] = 350;
	g_sv_h.val[2] = 350;
	g_sv_h.val[3] = 350;
      }
      if(__RC_ISPRESSED_CROSS(g_rc_data)){
	g_sv_h.val[0] = 150;
	g_sv_h.val[1] = 150;
	g_sv_h.val[2] = 150;
	g_sv_h.val[3] = 150;
      } 
    }
}
