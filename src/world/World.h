#ifndef WORLD_H
#define WORLD_H
#include <memory>
#include <vector>

#include "entities/MainPlayer.h"
#include "entities/StrafingTarget.h"

class World {

public:
  MainPlayer player;
  std::vector<std::shared_ptr<StrafingTarget>> targets;

};

#endif //WORLD_H
