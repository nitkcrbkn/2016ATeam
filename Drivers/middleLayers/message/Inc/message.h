#ifndef __MESSAGE_H
#define __MESSAGE_H
#include "xprintf.h"

#define message(type,fmt, ...) _msg(type,__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

void _msg(const char* type,
	  const char* file,
	  const char* func,
	  int line,
	  const char* fmt,
	  ...);

void MW_printf(const char* fmt,...);

void flush(void);

#endif
