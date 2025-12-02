#ifndef DRILL_TRACK_LINE_H
#define DRILL_TRACK_LINE_H

#include "UtilDrill.h"
#include "drill/controller/DrillController.h"
#include "shared/TrackController.h"
#include "shared/TimeController.h"
#include "world_controllers/CameraController.h"

struct DrillTrackLineParams {
  int duration = 60;
  float yaw = 60;
  float pitch = 30;
  float minDist = 5;
  float maxDist = 20;
  float period = 10;
  float slopeChancePitch = 0.2f;
  float slopeChanceRoll = 0.2f;
  float slopePitchMin = 10;
  float slopePitchMax = 45;
  float slopeRollMin = 20;
  float slopeRollMax = 45;

  // 5.4f * 0.6 is the rifle/sherif walking speed
  // (see "29.11.2025 What is the Valorant walking speed?" and
  // https://www.reddit.com/r/VALORANT/comments/utc7qf/valorant_physics_derived/)
  float maxVelocity = 5.4f * 0.6f;
  int killMs = 400;
  float targetSize = 0.2f;
};

class DrillTrackLine :
    public DrillController,
    public Controller,
    public std::enable_shared_from_this<DrillTrackLine> {
  // params
  DrillTrackLineParams params;

  // args
  DrillControllerSetupArgs args;

  // controllers
  std::shared_ptr<TimeController> ctrlTime;
  std::shared_ptr<CameraController> ctrlCam;
  std::shared_ptr<TrackController> ctrlTrack;

  // controller state
  bool over = false;
  DrillRandom rnd;

public:
  explicit DrillTrackLine(const std::string &args);

  // Own events

  void handleTime(const std::string &time);

  void handleTimeStart();

  void handleTimeEnd();

  void handleKilled(const std::shared_ptr<HitTarget>& target);

  //
  // drill controller
  //

  void setup(const DrillControllerSetupArgs &args) override;

  void pause() override;

  void resume() override;

  //
  // world controller
  //

  void update(double dt) override;

  void handle(const UiEvent &event) override;

  //
  // controller methods
  //
private:
  void spawn();
};

#endif //DRILL_TRACK_LINE_H
