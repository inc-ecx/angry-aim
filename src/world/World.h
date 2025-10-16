#ifndef WORLD_H
#define WORLD_H

#include "Entity.h"

#include <memory>
#include <set>

class World {

public:
  std::set<std::shared_ptr<Entity>> entities;

};

#endif //WORLD_H
