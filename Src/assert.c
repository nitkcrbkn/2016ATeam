#include "stm32f1xx_hal.h"

#define UNUSE(x) ((void)x)

void assert_failed(uint8_t *file, uint32_t line) {
  UNUSE(file);
  UNUSE(line);

  while(1);
}
