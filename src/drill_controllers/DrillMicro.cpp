#include "DrillMicro.h"

#include "Application.h"
#include "screens/ScreenResult.h"
#include "world/WorldUtil.h"

#include "world_controllers/CameraController.h"
#include "world_controllers/MoveController.h"

DrillMicro::DrillMicro(const std::string &args) :
  DrillController("micro") {
  float size;
  auto [ptr,ec] = std::from_chars(args.data(), args.data() + args.size(), size);
  if (ec == std::errc()) {
    params.targetSpawnWidth = size;
    params.targetMinHeight = 1.5f - size * 0.5f;
    params.targetMaxHeight = 1.5f + size * 0.5f;
  } // else error

  rngEngine.seed(static_cast<uint32_t>(msCurrent()));
}

void DrillMicro::setup(
  std::shared_ptr<WorldController> world, std::shared_ptr<MainPlayer> player,
  std::shared_ptr<UiDrill> screen
) {
  this->world = world;
  this->player = player;
  this->screen = screen;

  player->pos = glm::vec3(0.0f, params.eyeHeight, 0.0f);

  world->control(shared_from_this());
  world->control(player, cameraController = std::make_shared<CameraController>(player));
  world->control(player, moveController = std::make_shared<MoveController>(player, player));
  moveController->setEnabled(false);

  screen->lblMainStat->setText("Click to start");
}

void DrillMicro::update(double dt) {
  updateCheckSpawn(dt);
  updateClock(dt);
}

void DrillMicro::updateCheckSpawn(double dt) {
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

void DrillMicro::updateClock(double dt) {
  if (!started) return;
  if (stopped) return;

  uint64_t runningMs = msCurrent() - msStart;
  int timeLeft = static_cast<int>((params.duration * 1000 - static_cast<int>(runningMs)) / 1000.0f + 0.5f);
  if (timeLeft > 0) {
    int m = timeLeft / 60;
    int s = timeLeft % 60;
    screen->lblMainStat->setText(std::format("{:02d}:{:02d}", m, s));
  } else {
    screen->lblMainStat->setText("00:00");
    triggerStop();
  }
}

void DrillMicro::actionStart() {
  started = true;
  msStart = msCurrent();

  msToSpawn = msCurrent() + static_cast<uint64_t>(params.initialSpawnDelay * 1000);
}

void DrillMicro::triggerStop() {
  stopped = true;

  auto &app = Application::app;
  int m = params.duration / 60;
  int s = params.duration % 60;
  std::vector stats{
    std::format("Time: {:02d}:{:02d}", m, s),
    std::format("Hit: {}", statsHit),
    std::format("TTK: {}ms", statsTtkSum / statsHit),
    std::format("Hit Rate: {:0.1f}%", static_cast<float>(statsHit) / (statsHit + statsMissed) * 100.0f),
  };
  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  app.later(
    [stats] {
      Application::app.setScreen(std::make_shared<ScreenResult>(stats));
    }
  );

  cameraController->setEnabled(false);
}

void DrillMicro::actionShoot() {
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
    return;
  }

  statsHit++;
  statsTtkSum += static_cast<int>(msCurrent() - msLastSpawn);

  world->remove(target);
  target = nullptr;
  msToSpawn = msCurrent() + static_cast<uint64_t>(params.spawnDelay * 1000);
}

void DrillMicro::handle(const UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON && event.down && event.button == GLFW_MOUSE_BUTTON_LEFT) {
    handleLeftClick();
  }
}

void DrillMicro::handleLeftClick() {
  if (!started && !stopped) {
    actionStart();
  } else {
    if (started && !stopped) actionShoot();
  }
}
