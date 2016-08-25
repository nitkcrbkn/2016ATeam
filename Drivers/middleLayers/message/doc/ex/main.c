#include "message.h"

void foo(int arg);
void hoge(const char *str);

int main(void){
  flush();
  MW_printf("HelloWorld!!\n");
  MW_printf("This is demo of my printf!!\n");
  MW_printf("%%dformat :%d\n", 114514);
  MW_printf("%%bformat :%b\n", 1919810);

  message("message", "in function main");
  foo(5);
  foo(-6);
  hoge("foo!!");
  hoge("");
  flush();
  message("message", "helloWorld%d%s", 114514, "1919810");
  flush();
}

void foo(int arg){
  if( arg < 0 ){
    message("warning", "%d is under 0", arg);
  }else {
    message("warning", "%d is over(equal) 0", arg);
  }
}

