#include "helpers.h"

inline int32_t log2ceil(int32_t value) {
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
