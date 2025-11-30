#ifndef HIT_TARGET_H
#define HIT_TARGET_H
#include "components/Positioned.h"
#include "util/time_util.h"
#include "world/model/Entity.h"

class HitTarget :
    public Entity,
    public Positioned {
public:
  float size = 0.2f;
  uint64_t msDeath = 0;
  uint64_t msCreate = msCurrent();
  int fadeOutMs = 200;
  int fadeInMs = 0;
  bool isTrackable = false;
  int trackLifeMs = 400;
  bool isTracked = false;
  int trackedMs = 0;

  bool isHittable() {
    return msDeath == 0;
  }
};

#endif //HIT_TARGET_H
