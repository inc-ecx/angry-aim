#include "PatternStrafeController.h"

#include <algorithm>
#include "DebugInfo.h"
#include "Log.h"

PatternStrafeController::PatternStrafeController(
  const std::shared_ptr<HitTarget> &target,
  PatternStrafeControllerArgs &&args
) :
  target(target),
  args(args) {
  velocity = args.startSpeed;
  msPlayAction = msCurrent();
  if (args.pattern.items.size() < 2) throw std::runtime_error("pattern size < 2");
  if (args.startReversed) {
    playIndex = static_cast<int>(args.pattern.items.size() - 1);
    playDirection = -1;
  }
}

void PatternStrafeController::update(double dt) {
  if (args.isOver && args.isOver()) return;

  if (target->msDeath != 0) return;

  uint64_t msNow = msCurrent();
  uint64_t pastMs = msNow - msPlayAction;
  StrafeItem &item = args.pattern.items[playIndex];
  int timeUntilNextItem = item.duration - static_cast<int>(pastMs);
  if (timeUntilNextItem <= 0) {
    msPlayAction = msNow;
    playIndex += playDirection;
    if (playIndex == 0)
      playDirection = 1;
    if (playIndex == args.pattern.items.size() - 1)
      playDirection = -1;
  }

  double acceleration = item.acceleration * args.acceleration;
  velocity += acceleration * dt;
  velocity = std::clamp(velocity, static_cast<double>(-args.maxSpeed), static_cast<double>(args.maxSpeed));

  glm::vec3 deltaPos = args.direction * static_cast<float>(velocity * dt);
  target->pos += deltaPos;
}
