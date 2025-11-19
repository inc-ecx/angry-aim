#include "DrillSimple.h"

#include "Application.h"
#include "entities/Miss.h"
#include "screens/ScreenResult.h"
#include "state/State.h"
#include "world/WorldUtil.h"

#include "world_controllers/CameraController.h"
#include "world_controllers/MoveController.h"

DrillSimple::DrillSimple() :
  DrillController("simple") {
  rngEngine.seed(static_cast<uint32_t>(msCurrent()));
}

void DrillSimple::setup(const DrillControllerSetupArgs &args) {
  this->world = args.world;
  this->player = args.player;
  this->screen = args.screen;
  this->setupArgs = args;

  player->pos = glm::vec3(0.0f, params.eyeHeight, 0.0f);

  world->control(shared_from_this());
  world->control(player, cameraController = std::make_shared<CameraController>(player));
  world->control(player, moveController = std::make_shared<MoveController>(player, player));
  moveController->setEnabled(false);

  screen->lblMainStat->setText("Click to start");
}

void DrillSimple::pause() {
  cameraController->setEnabled(false);
}

void DrillSimple::resume() {
  cameraController->setEnabled(true);
}

void DrillSimple::update(double dt) {
  updateCheckSpawn(dt);
  updateRemoveDead(dt);
  updateClock(dt);
}

void DrillSimple::updateRemoveDead(double dt) {
  uint64_t msNow = msCurrent();

  // remove old target
  for (auto it = world->world.entities.begin(); it != world->world.entities.end();) {
    auto target = std::dynamic_pointer_cast<SimpleTarget>(*it);
    if (target && target->msDeath != 0 && static_cast<int64_t>(msNow - target->msDeath) > 0) {
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

void DrillSimple::updateCheckSpawn(double dt) {
  auto msNow = msCurrent();

  if (started && target == nullptr && msNow >= msToSpawn) {
    world->add(target = std::make_shared<SimpleTarget>());
    msLastSpawn = msNow;

    float x = rngDist(rngEngine) * params.targetSpawnWidth - 0.5f * params.targetSpawnWidth;
    float y = params.targetMinHeight + rngDist(rngEngine) * (params.targetMaxHeight - params.targetMinHeight);
    float z = params.targetMinDist + rngDist(rngEngine) * (params.targetMaxDist - params.targetMinDist);
    target->pos = glm::vec3(x, y, -z);
  }
}

void DrillSimple::updateClock(double dt) {
  if (!started) return;
  if (stopped) return;

  uint64_t runningMs = msCurrent() - msStart;
  int timeLeft = static_cast<int>((params.duration * 1000 - static_cast<int>(runningMs)) / 1000.0f + 0.5f);
  if (timeLeft > 0) {
    int m = timeLeft / 60;
    int s = timeLeft % 60;
    screen->lblMainStat->setText(std::format("{:02d}:{:02d}", m, s));
  } else {
    screen->lblMainStat->setText("");
    triggerStop();
  }
}

void DrillSimple::actionStart() {
  started = true;
  msStart = msCurrent();

  msToSpawn = msCurrent() + static_cast<uint64_t>(params.initialSpawnDelay * 1000);
}

void DrillSimple::triggerStop() {
  stopped = true;

  if (target != nullptr) {
    statsTtkSum += static_cast<int>(msCurrent() - msLastSpawn);
  }

  auto &app = Application::app;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // @formatter:off
  app.later([=] {
    Application::app.setScreen(std::make_shared<ScreenResult>(ScreenResultArgs{
      .drillProps = {
        {"drill", std::format("simple {}s", params.duration)}
      },
      .mainStats = {
        {"ttk", std::format("{}ms", statsTtkSum / std::max(1, statsHit))},
        {"acc", std::format("{:0.1f}%", static_cast<float>(statsHit) / std::max(1, statsHit + statsMissed) * 100.0f)},
        {"hit", std::format("{}", statsHit)},
      }
    }));
  });
  // @formatter:on

  cameraController->setEnabled(false);
}

void DrillSimple::actionShoot() {
  if (target == nullptr) return;

  float tHit = 0;
  bool didHit = WorldUtil::hitSphere(
    player->pos,
    static_cast<float>(player->pitch),
    static_cast<float>(player->yaw),
    target->pos,
    target->size * 0.5f, tHit
  );

  if (!didHit) {
    statsMissed++;
    setupArgs.resources->soundMiss->play(State::state.settings.missVolume);

    std::shared_ptr<Miss> miss;
    world->world.entities.insert(miss = std::make_shared<Miss>(msCurrent()));
    glm::vec3 dir;
    dir.x = cos(glm::radians(static_cast<float>(player->pitch))) * sin(
              glm::radians(static_cast<float>(player->yaw + 180))
            );
    dir.y = sin(glm::radians(static_cast<float>(player->pitch)));
    dir.z = cos(glm::radians(static_cast<float>(player->pitch))) * cos(
              glm::radians(static_cast<float>(player->yaw + 180))
            );
    dir = glm::normalize(dir);
    miss->pos = player->pos + dir;
    return;
  }

  statsHit++;
  statsTtkSum += static_cast<int>(msCurrent() - msLastSpawn);
  setupArgs.resources->soundHit->play(State::state.settings.hitVolume);

  target->msDeath = msCurrent() + target->fadeOutMs;
  target = nullptr;
  msToSpawn = msCurrent() + static_cast<uint64_t>(params.spawnDelay * 1000);
}

void DrillSimple::handle(const UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON && event.down && event.button == GLFW_MOUSE_BUTTON_LEFT) {
    handleLeftClick();
  }
}

void DrillSimple::handleLeftClick() {
  if (!started && !stopped) {
    actionStart();
  } else {
    if (started && !stopped) actionShoot();
  }
}
