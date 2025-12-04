#ifndef PATTERN_STRAFE_CONTROLLER_H
#define PATTERN_STRAFE_CONTROLLER_H

#include <functional>

#include "entities/HitTarget.h"
#include "world/controller/Controller.h"
#include "world_controllers/components/Toggelable.h"

struct StrafeItem {
  int duration; // [ms]
  float acceleration; // [acceleration units] (PatternStrafeControllerArgs.acceleration)
};

struct StrafePattern {
  std::vector<StrafeItem> items;
};

struct PatternStrafeControllerArgs {
  std::function<bool()> isOver;
  glm::vec3 direction; // normalized strafe direction
  float maxSpeed; // [m/s]
  float acceleration; // [m/s^2]
  StrafePattern pattern;
  bool startReversed;
  float startSpeed;
};

// replays the same movement pattern for a HitTarget. the pattern is reversed for even numbered replays.
class PatternStrafeController : public Controller {
  std::shared_ptr<HitTarget> target;
  PatternStrafeControllerArgs args;

  // pattern play state
  uint64_t msPlayAction = 0;
  int playIndex = 0;
  int playDirection = 1;

  // "physics" state
  double velocity = 0;

public:
  explicit PatternStrafeController(
    const std::shared_ptr<HitTarget> &target,
    PatternStrafeControllerArgs &&args
  );

  void update(double dt) override;
};

#endif //PATTERN_STRAFE_CONTROLLER_H
