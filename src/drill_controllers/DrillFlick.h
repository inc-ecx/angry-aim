#ifndef DRILL_FLICK_H
#define DRILL_FLICK_H

#include "UtilDrill.h"
#include "drill/controller/DrillController.h"
#include "world_controllers/CameraController.h"
#include "shared/HitController.h"

struct DrillFlickParams {
  int flicks = 50;
  float yaw = 30;
  float pitch = 30;
  float minDist = 5;
  float maxDist = 10;
};

class DrillFlick :
    public DrillController,
    public Controller,
    public std::enable_shared_from_this<DrillFlick> {
  DrillFlickParams params;
  DrillControllerSetupArgs args;
  std::shared_ptr<HitController> ctrlHit;
  std::shared_ptr<CameraController> ctrlCam;

  // state
  std::shared_ptr<HitTarget> lastTarget;
  bool started = false;
  uint64_t msStarted = false;
  bool paused = false;
  int flicksLeft = 0;
  bool flickStarted = false;
  uint64_t msMousePause = 0;
  DrillRandom rnd;

  void start();

  void end();

  void spawn();

public:
  DrillFlick();

  //
  // controller events
  //

  void handleMiss();

  void handleHit(const std::shared_ptr<HitTarget> &ptr);

  void handleLook();

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
};


#endif //DRILL_FLICK_H
