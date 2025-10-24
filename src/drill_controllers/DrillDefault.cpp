#include "DrillDefault.h"

#include "entities/StrafingTarget.h"
#include "scenes/SceneDrill.h"
#include "screens/ScreenResult.h"
#include "util/math_util.h"
#include "world/WorldUtil.h"
#include "world_controllers/CameraController.h"
#include "world_controllers/MoveController.h"

DrillDefault::DrillDefault() :
  DrillController("default") {}

void DrillDefault::setup(
  std::shared_ptr<WorldController> world,
  std::shared_ptr<MainPlayer> player,
  std::shared_ptr<UiDrill> screen
) {
  this->player = player;
  this->world = world;
  this->screen = screen;
  world->control(shared_from_this());
  world->control(player, cameraController = std::make_shared<CameraController>(player));
  // world->control(player, std::make_shared<MoveController>(player, player));

  player->pos = glm::vec3(0.0f, 1.6f, 0.0f);
  screen->lblMainStat->setText("Click to start");
}

void DrillDefault::update(double dt) {
  updateTimer();
  updateWorld(dt);
  updateSpawn(dt);
}

void DrillDefault::handle(const UiEvent &event) {
  if (!started) {
    if (event.type == UiEventType::MOUSE_BUTTON && event.button == GLFW_MOUSE_BUTTON_LEFT && event.down) {
      start();
    }
  }

  if (started && !over) {
    if (event.type == UiEventType::MOUSE_BUTTON && event.button == GLFW_MOUSE_BUTTON_LEFT && event.down) {
      bool hit = false;
      std::vector<std::shared_ptr<StrafingTarget> > targetsIg;
      for (auto e: world->world.entities) {
        auto strafingTarget = std::dynamic_pointer_cast<StrafingTarget>(e);
        if (strafingTarget) targetsIg.push_back(strafingTarget);
      }
      for (auto target: targetsIg) {
        float tHit = 0;
        bool didHit = WorldUtil::hitSphere(
          player->pos, static_cast<float>(player->pitch),
          static_cast<float>(player->yaw), target->pos, target->size * 0.5f, tHit
        );
        if (didHit) {
          world->remove(target);
          // ++it;
          hit = true;
          statsHit++;
          ttkSum += static_cast<int>(msCurrent() - msLastSpawn);
          triggerSpawn();
        }
      }
      if (!hit) statsMiss++;
    }
  }
}

void DrillDefault::start() {
  started = true;
  msStarted = msCurrent();
  triggerSpawn();
}

void DrillDefault::triggerSpawn() {
  spawnQueue++;
  spawnMsSpawn = msCurrent() + spawnDelay;
}

void DrillDefault::triggerOver() {
  over = true;
  cameraController->setEnabled(false);

  auto &app = Application::app;
  int m = duration / 60;
  int s = duration % 60;
  std::vector stats{
    std::format("Time: {:02d}:{:02d}", m, s),
    std::format("Hit: {}", statsHit),
    std::format("TTK: {}ms", ttkSum / statsHit),
    std::format("Hit Rate: {:0.1f}%", static_cast<float>(statsHit) / (statsHit + statsMiss) * 100.0f),
  };
  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  app.later(
    [stats] {
      Application::app.setScreen(std::make_shared<ScreenResult>(stats));
    }
  );
}

void DrillDefault::updateTimer() {
  if (!started) {
    screen->lblMainStat->setText("Click to start");
    return;
  }
  uint64_t runningMs = msCurrent() - msStarted;
  int timeLeft = static_cast<int>((duration * 1000 - static_cast<int>(runningMs)) / 1000.0f + 0.5f);
  if (timeLeft > 0) {
    int m = timeLeft / 60;
    int s = timeLeft % 60;
    screen->lblMainStat->setText(std::format("{:02d}:{:02d}", m, s));
  } else {
    screen->lblMainStat->setText("00:00");
    if (!over) triggerOver();
  }
}

void DrillDefault::updateSpawn(double dt) {
  uint64_t msNow = msCurrent();
  if (spawnQueue > 0 && static_cast<int64_t>(msNow - spawnMsSpawn) >= 0) {
    float mapWidth = 20;
    std::uniform_real_distribution<float> randomFloat;

    float strafeHeight = 1.5f;
    float strafeDist = spawnMinDist + randomFloat(rng) * (spawnMaxDist - spawnMinDist);
    float strafeWidth = spawnMinStrafe + randomFloat(rng) * (spawnMaxStrafe - spawnMinStrafe);

    float spawnSpace = mapWidth - strafeWidth;
    float spawnX = randomFloat(rng) * spawnSpace - spawnSpace * 0.5f;

    auto target = std::make_shared<StrafingTarget>();
    target->strafeStart = {spawnX, strafeHeight, -strafeDist};
    target->strafeWidth = strafeWidth;

    world->add(target);
    msLastSpawn = msCurrent();

    spawnQueue--;
  }
}

void DrillDefault::updateWorld(double dt) {
  if (!started || over) return;

  uint64_t msNow = msCurrent();

  bool doForceDirection = false;
  float input = 0;

  // apply acceleration/deceleration
  for (auto e: world->world.entities) {
    StrafingTarget *target = dynamic_cast<StrafingTarget *>(e.get());
    if (target == nullptr) continue;

    target->strafeVelocity = static_cast<float>(std::clamp(
      target->strafeVelocity + strafeAcceleration * target->strafeInput * dt, -strafeMaxSpeed,
      strafeMaxSpeed
    ));

    bool decelerating = std::abs(target->strafeInput) < 0.1 && std::abs(target->strafeVelocity) > 0.01;
    if (decelerating) {
      float velocityDir = target->strafeVelocity < 0 ? -1.0f : 1.0f;
      target->strafeVelocity = sub_nzc(
        target->strafeVelocity,
        static_cast<float>(velocityDir * dt * strafeDeceleration)
      );
    }
  }

  // apply velocity
  for (auto e: world->world.entities) {
    StrafingTarget *target = dynamic_cast<StrafingTarget *>(e.get());
    if (target == nullptr) continue;

    target->strafePos += target->strafeVelocity * static_cast<float>(dt);
    target->pos = target->strafeStart + glm::vec3(target->strafePos, 0, 0);
  }

  StrafingTarget *t = nullptr;

  // steer back to original position if max strafe width is hit
  for (auto e: world->world.entities) {
    StrafingTarget *target = dynamic_cast<StrafingTarget *>(e.get());
    if (target == nullptr) continue;
    t = target;

    float overshoot = target->strafePos - target->strafeWidth * 0.5f;
    if (overshoot > 0.3) {
      doForceDirection = true;
      input = -1;
    }
    float undershoot = -target->strafeWidth * 0.5f - target->strafePos;
    if (undershoot > 0.3) {
      doForceDirection = true;
      input = 1;
    }
  }

  // update strafe direction
  if (t != nullptr && (msNow > strafeMsSwitch || doForceDirection)) {
    std::uniform_int_distribution inputFunc(-1, 1);
    int randomInput = inputFunc(rng);
    t->strafeInput = static_cast<float>(randomInput);
    if (doForceDirection && randomInput != 0) t->strafeInput = input;

    std::uniform_int_distribution durationFunc(strafeMinDurationMs, strafeMaxDurationMs);
    strafeMsSwitch = msNow + durationFunc(rng);
  }
}
