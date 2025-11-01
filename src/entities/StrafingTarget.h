#ifndef STRAFING_TARGET_H
#define STRAFING_TARGET_H

#include "inc/inc_glm.h"
#include "world/model/Entity.h"

class StrafingTarget : public Entity {
public:
  float size = 0.2f;

  glm::vec3 pos = {}; // updated as pos = strafeStart + vec3(strafePos,0,0)

  glm::vec3 strafeStart = {}; // initial position
  float strafeWidth = 0; // width the strafe pos can deviate from strafe start

  float strafeInput = 0; // raw input (0, -1 or 1)
  float strafeVelocity = 0;
  float strafePos = 0;

  int fadeOutMs = 300;
  uint64_t msDeath = 0;
};

#endif //STRAFING_TARGET_H
