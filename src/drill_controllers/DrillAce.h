#ifndef DRILL_ACE_H
#define DRILL_ACE_H

#include <complex.h>
#include <random>

#include "drill/controller/DrillController.h"
#include "entities/HitTarget.h"
#include "shared/HitController.h"
#include "shared/TimeController.h"
#include "world_controllers/CameraController.h"

struct DrillAceParams {
  int duration = 60;
  float yaw = 60;
  float pitch = 30;
  float minDist = 3;
  float maxDist = 20;
  int waveCount = 5;
};

class DrillAce :
    public DrillController,
    public Controller,
    public std::enable_shared_from_this<DrillAce> {
  // params
  DrillAceParams params;

  // args
  DrillControllerSetupArgs args;

  // controllers
  std::shared_ptr<TimeController> ctrlTime;
  std::shared_ptr<CameraController> ctrlCam;
  std::shared_ptr<HitController> ctrlHit;

  // controller state
  std::default_random_engine rng;
  int living = 5;
  uint64_t msHitFirst = 0;

public:
  explicit DrillAce(const std::string &args);

  // Own events

  void handleMiss();

  void handleHit(const std::shared_ptr<HitTarget> &target);

  void handleTime(const std::string &time);

  void handleTimeStart();

  void handleTimeEnd();

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
  void spawnWave();
};

#endif //DRILL_ACE_H
