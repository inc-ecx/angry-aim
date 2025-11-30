#include "LineStrafeController.h"

#include "DebugInfo.h"
#include "Log.h"

LineStrafeController::LineStrafeController(
  const std::shared_ptr<HitTarget> &target,
  LineStrafeControllerArgs &&args
) :
  target(target),
  args(args) {
  this->period = glm::length(args.b - args.a);
}

void LineStrafeController::update(double dt) {
  if (args.isOver && args.isOver()) return;

  if (target->msDeath != 0) return;

  if (pos < 0) direction = 1;
  if (pos > period) direction = -1;
  pos += static_cast<float>(direction * args.speed * dt);
  // DebugInfo::put("miau miau", std::format("{} {}", pos, direction * args.speed * dt));
  // Log::info(std::format("{}", pos));

  glm::vec3 p = args.a + (args.b - args.a) * pos / period;
  target->pos = p;
}
