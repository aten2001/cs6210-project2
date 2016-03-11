#ifndef PROJECT2_HELPERS_H
#define PROJECT2_HELPERS_H

#include <stdlib.h>
#include <stdint.h>

static inline int32_t log2ceil(int32_t value) {
  if (value == 0) {
    return -1;
  }

  int32_t loop_val = value;
  int32_t count = 1;
  while (loop_val >>= 1) {
    ++count;
  }

  if (value > (1 << count)) {
    ++count;
  }
  return count;
}

static inline int mod(int a, int b) {
  int r = a % b;
  return r < 0 ? r + b : r;
}

#endif //PROJECT2_HELPERS_H
