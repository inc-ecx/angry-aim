#ifndef TARGET_H
#define TARGET_H
#include "components/Positioned.h"
#include "world/model/Entity.h"

class SimpleTarget :
    public Entity,
    public Positioned {
public:
  float size = 0.3f;
  uint64_t msDeath = 0;
  int fadeOutMs = 200;
};

#endif //TARGET_H
