#include "message.h"

void hoge(const char *str){
  if( *str == '\0' ){
    message("Error", "string is empty!!");
  }else {
    message("message", "I am %s.", str);
  }
}

