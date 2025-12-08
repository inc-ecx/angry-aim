#include "DrillStrafe.h"

#include "entities/StrafingTarget.h"
#include "entities/Miss.h"
#include "scenes/SceneDrill.h"
#include "screens/ScreenResult.h"
#include "shared/TimeController.h"
#include "state/State.h"
#include "util/math_util.h"
#include "world/WorldUtil.h"
#include "world_controllers/CameraController.h"
#include "world_controllers/MoveController.h"

DrillStrafe::DrillStrafe(const Drill &drill) :
  DrillController("default", drill) {}

void DrillStrafe::setup(const DrillControllerSetupArgs &args) {
  this->player = args.player;
  this->world = args.world;
  this->screen = args.screen;
  this->resources = args.resources;

  world->control(shared_from_this());
  auto handleTime = [this](const std::string& time) {
    screen->lblMainStat->setText(time);
  };
  auto handleStart = [this] {
    start();
  };
  auto handleEnd = [this] {
    triggerOver();
  };
  world->control(std::make_shared<TimeController>(TimeControllerArgs{duration, handleTime, handleStart, handleEnd}));
  world->control(player, cameraController = std::make_shared<CameraController>(player));
  // world->control(player, std::make_shared<MoveController>(player, player));

  player->pos = glm::vec3(0.0f, 1.6f, 0.0f);
  screen->lblMainStat->setText("Click to start");
}

void DrillStrafe::pause() {
  cameraController->setEnabled(false);
}

void DrillStrafe::resume() {
  cameraController->setEnabled(true);
}

void DrillStrafe::update(double dt) {
  updateTimer();
  updateWorld(dt);
  updateSpawn(dt);
}

void DrillStrafe::handle(const UiEvent &event) {
  if (started && !over) {
    if (event.type == UiEventType::MOUSE_BUTTON && event.button == GLFW_MOUSE_BUTTON_LEFT && event.down) {
      bool hit = false;
      std::vector<std::shared_ptr<StrafingTarget> > targetsIg;
      for (auto e: world->world.entities) {
        auto strafingTarget = std::dynamic_pointer_cast<StrafingTarget>(e);
        if (strafingTarget) targetsIg.push_back(strafingTarget);
      }
      for (auto target: targetsIg) {
        if (target->msDeath != 0) continue;

        float tHit = 0;
        bool didHit = WorldUtil::hitSphere(
          player->pos,
          static_cast<float>(player->pitch),
          static_cast<float>(player->yaw),
          target->pos,
          target->size * 0.5f,
          tHit
        );
        if (didHit) {
          // world->remove(target);
          // ++it;
          target->msDeath = msCurrent() + target->fadeOutMs;
          hit = true;
          statsHit++;
          resources->soundHit->play(State::state.settings.hitVolume);
          ttkSum += static_cast<int>(msCurrent() - msLastSpawn);
          triggerSpawn();
        }
      }
      if (!hit) {
        statsMiss++;
        resources->soundMiss->play(State::state.settings.missVolume);

        std::shared_ptr<Miss> miss;
        world->world.entities.insert(miss = std::make_shared<Miss>(msCurrent()));

        glm::vec3 dir;
        dir.x = cos(glm::radians(static_cast<float>(player->pitch))) * sin(glm::radians(static_cast<float>(player->yaw + 180)));
        dir.y = sin(glm::radians(static_cast<float>(player->pitch)));
        dir.z = cos(glm::radians(static_cast<float>(player->pitch))) * cos(glm::radians(static_cast<float>(player->yaw + 180)));
        dir = glm::normalize(dir);

        miss->pos = player->pos + dir;
      }
    }
  }
}

void DrillStrafe::start() {
  started = true;
  msStarted = msCurrent();
  triggerSpawn();
}

void DrillStrafe::triggerSpawn() {
  spawnQueue++;
  spawnMsSpawn = msCurrent() + spawnDelay;
}

void DrillStrafe::triggerOver() {
  screen->lblMainStat->setText("");
  over = true;
  cameraController->setEnabled(false);

  auto &app = Application::app;

  if (spawnQueue == 0) {
    ttkSum += static_cast<int>(msCurrent() - msLastSpawn);
  }

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  // @formatter:off
  app.later([=] {
    Application::app.setScreen(std::make_shared<ScreenResult>(ScreenResultArgs{
      .drill =  this->drill,
      .drillProps = {
        {"drill", std::format("strafe {}s", duration)}
      },
      .mainStats = {
        {"ttk", std::format("{}ms", ttkSum / std::max(1, statsHit))},
        {"acc", std::format("{:0.1f}%", static_cast<float>(statsHit) / std::max(1, statsHit + statsMiss) * 100.0f)},
        {"hit", std::format("{}", statsHit)},
      }
    }));
  });
  // @formatter:on
}

void DrillStrafe::updateTimer() {
}

void DrillStrafe::updateSpawn(double dt) {
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

void DrillStrafe::updateWorld(double dt) {
  if (!started || over) return;

  uint64_t msNow = msCurrent();

  bool doForceDirection = false;
  float input = 0;

  // apply acceleration/deceleration
  for (auto e: world->world.entities) {
    StrafingTarget *target = dynamic_cast<StrafingTarget *>(e.get());
    if (target == nullptr) continue;
    if (target->msDeath != 0) continue;

    target->strafeVelocity = static_cast<float>(std::clamp(
      target->strafeVelocity + strafeAcceleration * target->strafeInput * dt,
      -strafeMaxSpeed, strafeMaxSpeed
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
    if (target->msDeath != 0) continue;

    target->strafePos += target->strafeVelocity * static_cast<float>(dt);
    target->pos = target->strafeStart + glm::vec3(target->strafePos, 0, 0);
  }

  StrafingTarget *t = nullptr;

  // steer back to original position if max strafe width is hit
  for (auto e: world->world.entities) {
    StrafingTarget *target = dynamic_cast<StrafingTarget *>(e.get());
    if (target == nullptr) continue;
    if (target->msDeath != 0) continue;
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

  // remove old target
  // TODO: this is evil, it does directly access world model and not the world controller. could be in other drills too.
  for (auto it = world->world.entities.begin(); it != world->world.entities.end();) {
    auto strafingTarget = std::dynamic_pointer_cast<StrafingTarget>(*it);
    if (strafingTarget && strafingTarget->msDeath != 0 && static_cast<int64_t>(msNow - strafingTarget->msDeath) > 0) {
      it = world->world.entities.erase(it);
    } else {
      it++;
    }
  }

  // remove old miss
  for (auto it = world->world.entities.begin(); it != world->world.entities.end();) {
    auto miss = std::dynamic_pointer_cast<Miss>(*it);
    if (miss && static_cast<int64_t>(msNow - miss->msSpawn - miss->lifetimeMs) > 0) {
      it = world->world.entities.erase(it);
    } else {
      it++;
    }
  }
}
