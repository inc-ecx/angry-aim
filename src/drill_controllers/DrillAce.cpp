#include "DrillAce.h"

#include <random>

#include "Application.h"
#include "UtilDrill.h"
#include "screens/ScreenResult.h"
#include "shared/HitController.h"
#include "state/State.h"
#include "util/time_util.h"
#include "world/WorldUtil.h"
#include "world_controllers/CameraController.h"

DrillAce::DrillAce() :
  DrillController("ace") {
  rng.seed(static_cast<uint32_t>(msCurrent()));
}

//
// own events
//

void DrillAce::handleTime(const std::string &time) {
  args.screen->lblMainStat->setText(time);
}

void DrillAce::handleTimeStart() {
  spawnWave();
  ctrlHit->setEnabled(true);
}

void DrillAce::handleTimeEnd() {
  args.screen->lblMainStat->setText("");
  ctrlCam->setEnabled(false);
  ctrlHit->setEnabled(false);
  ctrlHit->notifyOver();

  auto &app = Application::app;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  UtilDrill::showResults(std::format("ace {}s", params.duration), ctrlHit);
}

//
// drill controller
//

// @formatter:off
void DrillAce::setup(const DrillControllerSetupArgs &args) {
  this->args = args;

  args.world->control(ctrlTime = std::make_shared<TimeController>(TimeControllerArgs{
    .durationSeconds = 60,
    .handleTime = std::bind(&DrillAce::handleTime, this, std::placeholders::_1),
    .handleStart = std::bind(&DrillAce::handleTimeStart, this),
    .handleEnd = std::bind(&DrillAce::handleTimeEnd, this)
  }));
  args.screen->lblMainStat->setText("Click to start");

  args.world->control(ctrlHit = std::make_shared<HitController>(HitControllerArgs{
    .world = args.world.get(),
    .player = args.player.get(),
    .listenerMiss = std::bind(&DrillAce::handleMiss, this),
    .listenerHit = std::bind(&DrillAce::handleHit, this, std::placeholders::_1)
  }));

  args.world->control(args.player, ctrlCam = std::make_shared<CameraController>(args.player));

  args.world->control(shared_from_this());
}
// @formatter:on

void DrillAce::handleMiss() {
  args.resources->soundMiss->play(State::state.settings.missVolume);
}

void DrillAce::handleHit(const std::shared_ptr<HitTarget> &target) {
  args.resources->soundHit->play(State::state.settings.hitVolume);
  living--;
  if (living > 0) {
    ctrlHit->notifyCanKill();
  }
}

void DrillAce::pause() {
  ctrlCam->setEnabled(false);
  ctrlHit->setEnabled(false);
}

void DrillAce::resume() {
  ctrlCam->setEnabled(true);
  ctrlHit->setEnabled(true);
}

//
// world controller
//

void DrillAce::update(double dt) {
  if (living == 0) {
    spawnWave();
  }
}

void DrillAce::handle(const UiEvent &event) {}

//
// controller methods
//
void DrillAce::spawnWave() {
  std::uniform_real_distribution<float> randomFloat;

  for (int i = 0; i < params.waveCount; ++i) {
    float rndPitch = params.pitch * (randomFloat(rng) - 0.5f);
    float rndYaw = params.yaw * (randomFloat(rng) - 0.5f);
    float rndDist = params.minDist + (randomFloat(rng) * (params.maxDist - params.minDist));

    glm::vec3 pos = WorldUtil::lookVec(rndPitch, rndYaw) * rndDist;

    auto target = std::make_shared<HitTarget>();
    target->pos = pos;
    args.world->add(target);
  }
  ctrlHit->notifyCanKill();
  living = params.waveCount;
}
