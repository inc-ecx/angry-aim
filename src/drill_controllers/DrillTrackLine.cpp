#include "DrillTrackLine.h"

#include <random>

#include "Application.h"
#include "UtilDrill.h"
#include "screens/ScreenResult.h"
#include "shared/LineStrafeController.h"
#include "shared/TrackController.h"
#include "state/State.h"
#include "world/WorldUtil.h"
#include "world_controllers/CameraController.h"

DrillTrackLine::DrillTrackLine(const json &args) :
  DrillController("track_line") {
  if (args.is_object() && args.contains("speed") && args["speed"].is_number()) {
    float speed = args["speed"];
    params.maxVelocity = speed;
  }
}

//
// own events
//

void DrillTrackLine::handleTime(const std::string &time) {
  args.screen->lblMainStat->setText(time);
}

void DrillTrackLine::handleTimeStart() {
  spawn();
  ctrlTrack->setEnabled(true);
}

void DrillTrackLine::handleTimeEnd() {
  args.screen->lblMainStat->setText("");
  ctrlCam->setEnabled(false);
  ctrlTrack->setEnabled(false);
  ctrlTrack->setCanKill(false);

  auto &app = Application::app;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  over = true;

  UtilDrill::showResults(std::format("track_line {}s {:.2f}m/s", params.duration, params.maxVelocity), ctrlTrack);
}

void DrillTrackLine::handleKilled(const std::shared_ptr<HitTarget> &target) {
  args.resources->soundHit->play(State::state.settings.hitVolume);
  spawn();
}

//
// drill controller
//

// @formatter:off
void DrillTrackLine::setup(const DrillControllerSetupArgs &args) {
  this->args = args;

  args.world->control(ctrlTime = std::make_shared<TimeController>(TimeControllerArgs{
    .durationSeconds = params.duration,
    .handleTime = std::bind(&DrillTrackLine::handleTime, this, std::placeholders::_1),
    .handleStart = std::bind(&DrillTrackLine::handleTimeStart, this),
    .handleEnd = std::bind(&DrillTrackLine::handleTimeEnd, this)
  }));
  args.screen->lblMainStat->setText("Click to start");

  args.world->control(ctrlTrack = std::make_shared<TrackController>(TrackControllerArgs{
    .world = args.world.get(),
    .player = args.player.get(),
    .handleKilled = std::bind(&DrillTrackLine::handleKilled, this, std::placeholders::_1),
  }));

  args.world->control(args.player, ctrlCam = std::make_shared<CameraController>(args.player));

  args.world->control(shared_from_this());
}
// @formatter:on

void DrillTrackLine::pause() {
  ctrlCam->setEnabled(false);
  ctrlTrack->setEnabled(false);
}

void DrillTrackLine::resume() {
  ctrlCam->setEnabled(true);
  ctrlTrack->setEnabled(true);
}

//
// world controller
//
void DrillTrackLine::update(double dt) {}

void DrillTrackLine::handle(const UiEvent &event) {}

//
// controller methods
//
void DrillTrackLine::spawn() {
  float pitch = rnd.nextFloat(-params.pitch * 0.5f, params.pitch * 0.5f);
  float yaw = rnd.nextFloat(-params.yaw * 0.5f, params.yaw * 0.5f);
  float dist = rnd.nextFloat(params.minDist, params.maxDist);
  bool dir = rnd.nextBool();

  float slopePitch = 0.0f;
  float slopeRoll = 0.0f;
  if (rnd.nextBool(params.slopeChancePitch))
    slopePitch = rnd.nextFloat(params.slopePitchMin, params.slopePitchMax);
  if (rnd.nextBool(params.slopeChanceRoll))
    slopeRoll = rnd.nextFloat(params.slopeRollMin, params.slopeRollMax);
  glm::mat4 rot = glm::yawPitchRoll(glm::radians(yaw), glm::radians(slopePitch), glm::radians(slopeRoll));
  glm::vec3 slant = glm::vec3(rot * glm::vec4(glm::vec3(1, 0, 0), 1.0f));
  glm::vec3 pos = args.player->pos + WorldUtil::lookVec(pitch, yaw) * dist;
  glm::vec3 a = pos - slant * (0.5f * params.period);
  glm::vec3 b = pos + slant * (0.5f * params.period);
  if (dir) std::swap(a, b);

  auto target = std::make_shared<HitTarget>();
  args.world->add(target);
  target->isTrackable = true;
  target->trackLifeMs = params.killMs;
  target->size = params.targetSize;
  target->pos = a;
  auto t = shared_from_this();

  // @formatter:off
  args.world->control(target, std::make_shared<LineStrafeController>(target, LineStrafeControllerArgs{
    .isOver = [t]() {return t->over;},
    .speed = params.maxVelocity,
    .a = a,
    .b = b,
  }));
  // @formatter:on
  ctrlTrack->setCanKill(true);
}
