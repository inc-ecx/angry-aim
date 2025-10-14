#ifndef STRAFING_TARGET_H
#define STRAFING_TARGET_H

#include "../../inc/inc_glm.h"

class StrafingTarget {
public:
  glm::vec3 pos = {};
  glm::vec3 min = {};
  glm::vec3 max = {};
  float velocity = 0;
  float input = 1;
  float size = 0.3f;
};

#endif //STRAFING_TARGET_H
