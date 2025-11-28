#include "DrillFlick.h"

#include "Application.h"
#include "state/State.h"
#include "world/WorldUtil.h"

DrillFlick::DrillFlick() :
  DrillController("flick") {}

void DrillFlick::handleMiss() {
  handleLook();

  args.resources->soundMiss->play(State::state.settings.missVolume);
}

void DrillFlick::handleHit(const std::shared_ptr<HitTarget> &ptr) {
  handleLook();

  args.resources->soundHit->play(State::state.settings.hitVolume);

  flicksLeft--;
  if (flicksLeft <= 0) {
    end();
    return;
  }
  args.screen->lblMainStat->setText(std::format("{}", flicksLeft));

  msMousePause = msCurrent() + 500;
  args.screen->lblMainStat->setRgba(0xff0000ff);
  ctrlCam->setEnabled(false);
  ctrlHit->setEnabled(false);
  spawn();
}

void DrillFlick::handleLook() {
  if (!started) return;

  if (!flickStarted) {
    flickStarted = true;
    args.screen->lblMainStat->setRgba(0xff8080ff);
    ctrlHit->notifyCanKill();
  }
}

void DrillFlick::setup(const DrillControllerSetupArgs &args) {
  this->args = args;

  flicksLeft = params.flicks;

  ctrlHit = std::make_shared<HitController>(
    HitControllerArgs{
      .world = args.world.get(),
      .player = args.player.get(),
      .listenerMiss = std::bind(&DrillFlick::handleMiss, this),
      .listenerHit = std::bind(&DrillFlick::handleHit, this, std::placeholders::_1)
    }
  );
  args.world->control(ctrlHit);

  ctrlCam = std::make_shared<CameraController>(
    args.player,
    std::bind(&DrillFlick::handleLook, this)
  );
  args.world->control(ctrlCam);

  args.world->control(shared_from_this());

  args.screen->lblMainStat->setText("Click to start");
}

void DrillFlick::pause() {
  ctrlCam->setEnabled(false);
  ctrlHit->setEnabled(false);
  paused = true;
}

void DrillFlick::resume() {
  ctrlCam->setEnabled(true);
  ctrlHit->setEnabled(true);
  paused = false;
}

void DrillFlick::update(double dt) {
  if (paused) return;

  if (msMousePause != 0 && msCurrent() > msMousePause) {
    ctrlCam->setEnabled(true);
    ctrlHit->setEnabled(true);
  }
}

void DrillFlick::handle(const UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON && event.button == GLFW_MOUSE_BUTTON_LEFT) {
    if (!started) {
      start();
    }
  }
}

void DrillFlick::spawn() {
  flickStarted = false;
  args.screen->lblMainStat->setRgba(0xffffffff);

  float pitch = rnd.nextFloat(-params.pitch * 0.5f, params.pitch * 0.5f);
  float yaw = rnd.nextFloat(-params.yaw * 0.5f, params.yaw * 0.5f);
  float dist = rnd.nextFloat(params.minDist, params.maxDist);
  glm::vec3 pos = WorldUtil::lookVec(pitch, yaw) * dist;

  auto target = std::make_shared<HitTarget>();
  target->pos = pos;
  target->fadeInMs = 600;
  args.world->add(target);
}

void DrillFlick::start() {
  args.screen->lblMainStat->setText(std::format("{}", flicksLeft));
  started = true;
  msStarted = msCurrent();
  ctrlCam->setEnabled(true);
  ctrlHit->setEnabled(true);
  spawn();
}

void DrillFlick::end() {
  args.screen->lblMainStat->setText("");
  ctrlCam->setEnabled(false);
  ctrlHit->setEnabled(false);
  ctrlHit->notifyOver();

  auto &app = Application::app;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  uint64_t durationMs = msCurrent() - msStarted;
  UtilDrill::showResultsHit(std::format("flick {}", params.flicks), ctrlHit, durationMs);
}