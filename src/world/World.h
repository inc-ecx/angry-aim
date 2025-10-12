#ifndef WORLD_H
#define WORLD_H
#include <vector>

#include "Entity.h"

class World {
public:
  std::vector<Entity> entities;
};

#endif //WORLD_H
