#ifndef DRILL_CONTROLLER_H
#define DRILL_CONTROLLER_H

#include <memory>
#include <string>

#include "drill/model/Drill.h"
#include "entities/MainPlayer.h"
#include "scenes/SceneDrillResources.h"
#include "scenes/UiDrill.h"
#include "world/controller/WorldController.h"

struct DrillControllerSetupArgs {
  std::shared_ptr<WorldController> world;
  std::shared_ptr<MainPlayer> player;
  std::shared_ptr<UiDrill> screen;
  std::shared_ptr<SceneDrillResources> resources;
};

class DrillController {
public:
  std::string id;
  const Drill drill;

  DrillController(const std::string &id, const Drill& drill) :
    id(id), drill(drill) {}

  virtual ~DrillController() = default;

  virtual void setup(
    const DrillControllerSetupArgs &args
  ) {}

  virtual void pause() {}

  virtual void resume() {}
};

#endif // DRILL_CONTROLLER_H
