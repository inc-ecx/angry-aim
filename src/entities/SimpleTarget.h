#ifndef TARGET_H
#define TARGET_H
#include "components/Positioned.h"
#include "world/model/Entity.h"

class SimpleTarget :
    public Entity,
    public Positioned {
public:
  float size = 0.3f;
};

#endif //TARGET_H
