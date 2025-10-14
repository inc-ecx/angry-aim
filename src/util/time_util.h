#ifndef TIME_UTIL_H
#define TIME_UTIL_H

#include <chrono>

inline uint64_t msCurrent() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

#endif //TIME_UTIL_H
