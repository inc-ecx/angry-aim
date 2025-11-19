#ifndef DRILL_DEFAULT_H
#define DRILL_DEFAULT_H

#include "Application.h"
#include <memory>
#include <random>

#include "drill/controller/DrillController.h"
#include "world_controllers/CameraController.h"

class DrillStrafe :
    public DrillController,
    public Controller,
    public std::enable_shared_from_this<DrillStrafe> {
  std::shared_ptr<WorldController> world;
  std::shared_ptr<MainPlayer> player;
  std::shared_ptr<UiDrill> screen;
  std::shared_ptr<SceneDrillResources> resources;
  
  // Valorant Physics Analyzed:
  // https://www.reddit.com/r/VALORANT/comments/utc7qf/valorant_physics_derived/

  std::default_random_engine rng;

  int duration = 60;

  // spawning
  int spawnDelay = 100;
  float spawnMinDist = 5;
  float spawnMaxDist = 30;
  float spawnMinStrafe = 1;
  float spawnMaxStrafe = 10;
  int spawnQueue = 0;
  uint64_t spawnMsSpawn = 0; // timestamp by when a spawn can happen

  // strafing
  int strafeMinDurationMs = 150;
  int strafeMaxDurationMs = 500;
  double strafeAcceleration = 18.75;
  double strafeMaxSpeed = 5.4;
  double strafeDeceleration = 28.6;
  uint64_t strafeMsSwitch = 0; // timestamp by when the strafe direction needs to be switched

  // stats
  int statsHit = 0;
  int statsMiss = 0;
  int ttkSum = 0;
  uint64_t msLastSpawn = 0;

  // game state
  uint64_t msStarted = 0;
  bool started = false;
  bool over = false;
  std::shared_ptr<CameraController> cameraController;

  void updateWorld(double dt);

  void start();

  void triggerSpawn();

  void triggerOver();

  void updateTimer();

  void updateSpawn(double dt);

public:
  explicit DrillStrafe();

  void setup(const DrillControllerSetupArgs &args) override;

  void pause() override;

  void resume() override;

  void update(double dt) override;

  void handle(const UiEvent &event) override;
};

#endif //DRILL_DEFAULT_H
