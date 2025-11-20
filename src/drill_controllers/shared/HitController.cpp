#include "HitController.h"

#include "Application.h"
#include "Log.h"
#include "entities/HitTarget.h"
#include "entities/Miss.h"
#include "world/WorldUtil.h"

HitController::HitController(HitControllerArgs &&args) :
  Toggelable(false),
  args(args) {
  msLastSpawn = 0;
}

void HitController::update(double dt) {
  if (!isEnabled()) return;

  uint64_t msNow = msCurrent();
  std::vector<std::shared_ptr<HitTarget>> toRemove;
  for (auto entity : args.world->world.entities) {
    auto target = std::dynamic_pointer_cast<HitTarget>(entity);
    if (target && target->msDeath != 0 && static_cast<int64_t>(msNow - target->msDeath) > 0) {
      toRemove.push_back(target);
    }
  }

  for (auto removed : toRemove) {
    args.world->remove(removed);
  }
}

void HitController::handle(const UiEvent &event) {
  if (!isEnabled()) return;

  if (event.type == UiEventType::MOUSE_BUTTON && event.button == GLFW_MOUSE_BUTTON_LEFT && event.down) {
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
        hit = true;
        onHit(target);
      }
    }
    if (!hit) {
      onMiss();
    }
  }
}

void HitController::notifyCanKill() {
  msLastSpawn = msCurrent();
}

void HitController::notifyOver() {
  if (msLastSpawn == 0) {
    Log::warn("Failed to call HitController::notifySpawn.");
  } else {
    stats.ttkSum += static_cast<int>(msCurrent() - msLastSpawn);
  }
}

void HitController::onMiss() {
  stats.miss++;

  std::shared_ptr<Miss> miss;

  args.world->world.entities.insert(miss = std::make_shared<Miss>(msCurrent()));

  glm::vec3 dir;
  dir.x = cos(glm::radians(static_cast<float>(args.player->pitch)))
          * sin(glm::radians(static_cast<float>(args.player->yaw + 180)));
  dir.y = sin(glm::radians(static_cast<float>(args.player->pitch)));
  dir.z = cos(glm::radians(static_cast<float>(args.player->pitch)))
          * cos(glm::radians(static_cast<float>(args.player->yaw + 180)));
  dir = glm::normalize(dir);

  miss->pos = args.player->pos + dir;

  if (args.listenerMiss) args.listenerMiss();
}

void HitController::onHit(const std::shared_ptr<HitTarget> &target) {
  stats.hit++;
  target->msDeath = msCurrent() + target->fadeOutMs;

  if (msLastSpawn == 0) {
    Log::warn("Failed to call HitController::notifySpawn.");
  } else {
    stats.ttkSum += static_cast<int>(msCurrent() - msLastSpawn);
  }

  if (args.listenerHit) args.listenerHit(target);
}
