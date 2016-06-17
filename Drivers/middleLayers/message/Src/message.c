#include "xprintf.h"
#include <stdio.h>
#include <stdarg.h>

#define message(type,fmt, ...) _msg(type,__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

static
void _xprintf(char *fmt,...);

static
char buff[1024];


void MW_printf(const char* fmt,...){
    va_list arp;
    va_start(arp, fmt);
    xvprintf(fmt, arp);
    va_end(arp);
}

void _msg(const char* type,
	  const char* file,
	  const char* func,
	  int line,
	  const char* fmt,
	  ...){
  _xprintf("%-10s %-10s %-3d %-8s ",file,func,line,type);

  {
    va_list arp;
    va_start(arp, fmt);
    xvprintf(fmt, arp);
    va_end(arp);
  }

  _xprintf("\n");
}

void flush(void){
  if(outptr!=0){
    *outptr = 0;		/* Terminate output string with a \0 */
    printf("%s",buff);		/*print string.*/
  }
  outptr=buff;
}

static
void _xprintf(char *fmt,...){
  va_list arp;
  va_start(arp, fmt);
  xvprintf(fmt, arp);
  va_end(arp);
}
