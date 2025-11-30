#ifndef LINE_STRAFE_CONTROLLER_H
#define LINE_STRAFE_CONTROLLER_H

#include <functional>

#include "entities/HitTarget.h"
#include "world/controller/Controller.h"
#include "world_controllers/components/Toggelable.h"

struct LineStrafeControllerArgs {
  std::function<bool()> isOver;
  float speed;
  glm::vec3 a;
  glm::vec3 b;
};

class LineStrafeController : public Controller {
  std::shared_ptr<HitTarget> target;
  LineStrafeControllerArgs args;
  float period;
  float pos = 0;
  float direction = 1;

public:
  explicit LineStrafeController(
    const std::shared_ptr<HitTarget> &target,
    LineStrafeControllerArgs &&args
  );

  void update(double dt) override;
};

#endif //LINE_STRAFE_CONTROLLER_H
