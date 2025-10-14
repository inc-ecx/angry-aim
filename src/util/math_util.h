#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "inc/inc_glm.h"

inline float sub_nzc(float a, float b) {
  float r = a - b;
  if (a < 0.0f != r < 0.0f) return 0.0f;
  return r;
}

inline glm::vec3 sub_nzc(glm::vec3 a, glm::vec3 b) {
  return glm::vec3(sub_nzc(a.x,b.x),sub_nzc(a.y,b.y),sub_nzc(a.z,b.z));
}

#endif //MATH_UTIL_H
