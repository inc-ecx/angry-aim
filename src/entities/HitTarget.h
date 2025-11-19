#ifndef HIT_TARGET_H
#define HIT_TARGET_H
#include "components/Positioned.h"
#include "world/model/Entity.h"

class HitTarget :
    public Entity,
    public Positioned {
public:
  float size = 0.2f;
  uint64_t msDeath = 0;
  int fadeOutMs = 200;

  bool isHittable() {
    return msDeath == 0;
  }
};

#endif //HIT_TARGET_H
