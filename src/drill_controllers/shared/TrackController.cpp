#include "TrackController.h"

#include "Application.h"
#include "entities/HitTarget.h"
#include "entities/Miss.h"
#include "world/WorldUtil.h"

TrackController::TrackController(TrackControllerArgs &&args) :
  Toggelable(false),
  args(args) {}

void TrackController::update(double dt) {
  for (auto e: args.world->world.entities) {
    auto target = std::dynamic_pointer_cast<HitTarget>(e);
    if (!target) continue;
    if (target->msDeath != 0) continue;
    if (target->trackedMs > target->trackLifeMs) {
      target->msDeath = msCurrent() + target->fadeOutMs;
      if (args.handleKilled) args.handleKilled(target);
    }
  }

  uint64_t msNow = msCurrent();
  std::vector<std::shared_ptr<HitTarget> > toRemove;
  for (auto entity: args.world->world.entities) {
    auto target = std::dynamic_pointer_cast<HitTarget>(entity);
    if (target && target->msDeath != 0 && static_cast<int64_t>(msNow - target->msDeath) > 0) {
      toRemove.push_back(target);
    }
  }
  for (auto removed: toRemove) {
    args.world->remove(removed);
  }

  for (auto e: args.world->world.entities) {
    auto target = std::dynamic_pointer_cast<HitTarget>(e);
    if (!target) continue;
    target->isTracked = false;
  }

  if (!isEnabled()) return;

  if (!canKill) return;

  uint64_t pastMs = msMeasure == 0 ? 0 : msNow - msMeasure;
  msMeasure = msNow;

  bool hit = false;
  for (auto e: args.world->world.entities) {
    auto target = std::dynamic_pointer_cast<HitTarget>(e);
    if (!target) continue;
    if (!target->isHittable()) continue;
    float tHit = 0;
    if (WorldUtil::hitSphere(
      args.player->pos,
      static_cast<float>(args.player->pitch),
      static_cast<float>(args.player->yaw),
      target->pos,
      target->size * 0.5f,
      tHit
    )) {
      target->isTracked = true;
      hit = true;
      target->trackedMs += static_cast<int>(pastMs);
    }
  }

  if (hit) {
    stats.hoverMs += pastMs;
    if (hasReachedTarget) stats.reachedHoverMs += pastMs;
    if (!hasReachedTarget) {
      onInitialReach();
      hasReachedTarget = true;
    }
  } else {
    stats.missMs += pastMs;
    if (hasReachedTarget) stats.reachedMissMs+=pastMs;
  }
}

void TrackController::onInitialReach() {
  uint64_t timeToReachMs = msCurrent() - msCanReach;
  stats.reachedCount++;
  stats.timeToReachSumMs += timeToReachMs;
}

void TrackController::handle(const UiEvent &event) {}

void TrackController::onDisable() {
  msMeasure = 0;
}

void TrackController::setCanKill(bool canKill) {
  if (canKill) {
    hasReachedTarget = false;
    msCanReach = msCurrent();
  }

  this->canKill = canKill;
  if (!this->canKill) {
    msMeasure = 0;
  }
}
