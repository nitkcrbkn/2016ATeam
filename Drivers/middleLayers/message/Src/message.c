#include "xprintf.h"
#include "MW_USART.h"
#include "message.h"
#include <stdarg.h>
#include <stdbool.h>

#define message(type, fmt, ...) _msg(type, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)

static
void _xprintf(char *fmt, ...);

static
char buff[1024];

static
volatile bool had_completed = true;

void MW_printf(const char* fmt, ...){
  va_list arp;
  va_start(arp, fmt);
  xvprintf(fmt, arp);
  va_end(arp);
  if(outptr-buff>MAX_STRING_LENGTH){
    flush();
  }
}

void _msg(const char* type,
          const char* func,
          int line,
          const char* fmt,
          ...){
  _xprintf("%-10s %-3d %-8s", func, line, type);

  {
    va_list arp;
    va_start(arp, fmt);
    xvprintf(fmt, arp);
    va_end(arp);
  }
  _xprintf("\n");

  flush();
}

void flush(void){
  while(!had_completed);
  if( outptr != 0 ){
    *outptr++ = '\n';
    MW_USART2Transmit((uint8_t*)buff, outptr - buff);
    had_completed = false;
  }
  outptr = buff;
}

static
void _xprintf(char *fmt, ...){
  va_list arp;
  va_start(arp, fmt);
  xvprintf(fmt, arp);
  va_end(arp);
}

void MW_hadCompleted(void){
  had_completed = true;
}
