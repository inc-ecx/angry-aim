#ifndef MISS_H
#define MISS_H
#include "components/Positioned.h"
#include "world/model/Entity.h"

class Miss :
    public Entity,
    public Positioned {
public:
  float sizePx = 1.0f;
  uint64_t msSpawn;
  int lifetimeMs = 400;

  explicit Miss(uint64_t msSpawn) : msSpawn(msSpawn) {}
};

#endif //MISS_H
