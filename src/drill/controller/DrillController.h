#ifndef DRILL_CONTROLLER_H
#define DRILL_CONTROLLER_H

#include <memory>
#include <string>

#include "entities/MainPlayer.h"
#include "screens/ScreenDrill.h"
#include "world/controller/WorldController.h"

class DrillController {
public:
  std::string id;

  explicit DrillController(const std::string &id)
    : id(id) {
  }

  virtual ~DrillController() = default;

  virtual void setup(
    std::shared_ptr<WorldController> world,
    std::shared_ptr<MainPlayer> player,
    std::shared_ptr<ScreenDrill> screen) {
  }
};

#endif // DRILL_CONTROLLER_H
