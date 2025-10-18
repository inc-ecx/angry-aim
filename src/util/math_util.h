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

inline float smoothStep(float edge0, float edge1, float x) {
  x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
  return x * x * (3.0f - 2.0f * x);
}

#endif //MATH_UTIL_H
