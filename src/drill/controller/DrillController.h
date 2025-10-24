#ifndef DRILL_CONTROLLER_H
#define DRILL_CONTROLLER_H

#include <memory>
#include <string>

#include "entities/MainPlayer.h"
#include "scenes/UiDrill.h"
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
    std::shared_ptr<UiDrill> screen) {
  }
};

#endif // DRILL_CONTROLLER_H
