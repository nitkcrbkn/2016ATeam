#ifndef __MESSAGE_H
#define __MESSAGE_H

//auto transmit.(>900)
#define MAX_STRING_LENGTH 900 

#define message(type, fmt, ...) _msg(type, __FUNCTION__, __LINE__, fmt, ## __VA_ARGS__)
void _msg(const char* type,
	  const char* func,
	  int line,
	  const char* fmt,
	  ...);

void MW_printf(const char* fmt,...);

void flush(void);

#endif
