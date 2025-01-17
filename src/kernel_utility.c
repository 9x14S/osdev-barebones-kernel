#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel_utility.h"

size_t strlen(const char *str)
{
  int i;
  for (i = 0; str[i]; i++);
  return i;
}
