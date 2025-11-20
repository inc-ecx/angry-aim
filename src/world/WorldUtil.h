#ifndef WORLD_UTIL_H
#define WORLD_UTIL_H

#include "inc/inc_glm.h"

class WorldUtil {
public:
  static bool hitSphere(
    const glm::vec3 &rayOrigin,
    float pitch,
    float yaw,
    const glm::vec3 &spherePos,
    float radius,
    float &tHit
  );

  static glm::vec3 lookVec(float pitch, float yaw);
};


#endif //WORLD_UTIL_H
