#ifndef DRILL_MICRO_H
#define DRILL_MICRO_H

#include <random>

#include "drill/controller/DrillController.h"
#include "entities/SimpleTarget.h"
#include "world_controllers/CameraController.h"
#include "world_controllers/MoveController.h"

class DrillMicro :
    public DrillController,
    public Controller,
    public std::enable_shared_from_this<DrillMicro> {
  std::shared_ptr<UiDrill> screen;
  std::shared_ptr<WorldController> world;
  std::shared_ptr<MainPlayer> player;
  std::shared_ptr<CameraController> cameraController;
  std::shared_ptr<MoveController> moveController;

  // Drill settings
  struct Params {
    int duration = 60;
    double initialSpawnDelay = 0.2;
    double spawnDelay = 0.0;
    float eyeHeight = 1.6f;

    float targetMinHeight = 1.0;
    float targetMaxHeight = 3.5;
    float targetMinDist = 30;
    float targetMaxDist = 40;
    float targetSpawnWidth = 4;
  } params;

  // Drill game state
  bool started = false;
  bool stopped = false;
  uint64_t msStart = 0;

  // Current target data
  std::default_random_engine rngEngine;
  std::uniform_real_distribution<float> rngDist;
  std::shared_ptr<SimpleTarget> target;
  uint64_t msToSpawn = 0;
  uint64_t msLastSpawn = 0;

  // Statistics
  int statsHit = 0;
  int statsMissed = 0;
  int statsTtkSum = 0;

  void triggerStop();

  void actionStart();

  void actionShoot();

  void updateCheckSpawn(double dt);

  void updateClock(double dt);

  void handleLeftClick();

public:
  explicit DrillMicro(const std::string &args);

  void setup(std::shared_ptr<WorldController> world, std::shared_ptr<MainPlayer> player,
             std::shared_ptr<UiDrill> screen) override;

  void update(double dt) override;

  void handle(const UiEvent &event) override;
};


#endif //DRILL_MICRO_H
