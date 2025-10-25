#ifndef SCENE_DRILL_RESOURCES_H
#define SCENE_DRILL_RESOURCES_H

#include "sound/Sound.h"

struct SceneDrillResources {
  std::shared_ptr<Sound> soundHit;
  std::shared_ptr<Sound> soundMiss;
};

#endif //SCENE_DRILL_RESOURCES_H
