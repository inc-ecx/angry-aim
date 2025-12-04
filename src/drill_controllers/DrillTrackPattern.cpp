#include "DrillTrackPattern.h"

#include <random>

#include "Application.h"
#include "UtilDrill.h"
#include "screens/ScreenResult.h"
#include "shared/PatternStrafeController.h"
#include "shared/TrackController.h"
#include "state/State.h"
#include "world/WorldUtil.h"
#include "world_controllers/CameraController.h"

DrillTrackPattern::DrillTrackPattern(const std::string &args) :
  DrillController("track_pattern") {
  float speed;
  auto [ptr,ec] = std::from_chars(args.data(), args.data() + args.size(), speed);
  if (ec == std::errc()) {
    params.maxVelocity = speed;
  }
}

//
// own events
//

void DrillTrackPattern::handleTime(const std::string &time) {
  args.screen->lblMainStat->setText(time);
}

void DrillTrackPattern::handleTimeStart() {
  spawn();
  ctrlTrack->setEnabled(true);
}

void DrillTrackPattern::handleTimeEnd() {
  args.screen->lblMainStat->setText("");
  ctrlCam->setEnabled(false);
  ctrlTrack->setEnabled(false);
  ctrlTrack->setCanKill(false);

  auto &app = Application::app;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  over = true;

  UtilDrill::showResults(std::format("track_pattern {}s {:.2f}m/s", params.duration, params.maxVelocity), ctrlTrack);
}

void DrillTrackPattern::handleKilled(const std::shared_ptr<HitTarget> &target) {
  args.resources->soundHit->play(State::state.settings.hitVolume);
  spawn();
}

//
// drill controller
//

// @formatter:off
void DrillTrackPattern::setup(const DrillControllerSetupArgs &args) {
  this->args = args;

  args.world->control(ctrlTime = std::make_shared<TimeController>(TimeControllerArgs{
    .durationSeconds = params.duration,
    .handleTime = std::bind(&DrillTrackPattern::handleTime, this, std::placeholders::_1),
    .handleStart = std::bind(&DrillTrackPattern::handleTimeStart, this),
    .handleEnd = std::bind(&DrillTrackPattern::handleTimeEnd, this)
  }));
  args.screen->lblMainStat->setText("Click to start");

  args.world->control(ctrlTrack = std::make_shared<TrackController>(TrackControllerArgs{
    .world = args.world.get(),
    .player = args.player.get(),
    .handleKilled = std::bind(&DrillTrackPattern::handleKilled, this, std::placeholders::_1),
  }));

  args.world->control(args.player, ctrlCam = std::make_shared<CameraController>(args.player));

  args.world->control(shared_from_this());
}
// @formatter:on

void DrillTrackPattern::pause() {
  ctrlCam->setEnabled(false);
  ctrlTrack->setEnabled(false);
}

void DrillTrackPattern::resume() {
  ctrlCam->setEnabled(true);
  ctrlTrack->setEnabled(true);
}

//
// world controller
//
void DrillTrackPattern::update(double dt) {}

void DrillTrackPattern::handle(const UiEvent &event) {}

//
// controller methods
//
void DrillTrackPattern::spawn() {
  auto sThis = shared_from_this();

  float pitch = rnd.nextFloat(-params.pitch * 0.5f, params.pitch * 0.5f);
  float yaw = rnd.nextFloat(-params.yaw * 0.5f, params.yaw * 0.5f);
  float dist = rnd.nextFloat(params.minDist, params.maxDist);

  float slopePitch = 0.0f;
  float slopeRoll = 0.0f;
  if (rnd.nextBool(params.slopeChancePitch))
    slopePitch = rnd.nextFloat(params.slopePitchMin, params.slopePitchMax);
  if (rnd.nextBool(params.slopeChanceRoll))
    slopeRoll = rnd.nextFloat(params.slopeRollMin, params.slopeRollMax);

  bool startReversedVal = rnd.nextBool();
  float startVelocityVal = rnd.nextFloat(-params.maxVelocity, params.maxVelocity);

  glm::mat4 rot = glm::yawPitchRoll(glm::radians(yaw), glm::radians(slopePitch), glm::radians(slopeRoll));
  glm::vec3 direction = glm::vec3(rot * glm::vec4(glm::vec3(1, 0, 0), 1.0f));
  glm::vec3 pos = args.player->pos + WorldUtil::lookVec(pitch, yaw) * dist;

  auto target = std::make_shared<HitTarget>();
  target->isTrackable = true;
  target->trackLifeMs = params.killMs;
  target->size = params.targetSize;
  target->pos = pos;
  args.world->add(target);

  // @formatter:off
  args.world->control(target, std::make_shared<PatternStrafeController>(target, PatternStrafeControllerArgs{
    .isOver = [sThis]() {return sThis->over;},
    .direction = direction,
    .maxSpeed = params.maxVelocity,
    .acceleration = params.strafeAcceleration,
    .pattern = StrafePattern{
      .items = {
        StrafeItem{.duration = 500, .acceleration = 1.0f},
        StrafeItem{.duration = 500, .acceleration = -1.0f},
      }
    },
    .startReversed = startReversedVal,
    .startSpeed = startVelocityVal,
  }));
  // @formatter:on
  ctrlTrack->setCanKill(true);
}
