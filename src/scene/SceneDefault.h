#ifndef SCENE_DEFAULT_H
#define SCENE_DEFAULT_H

#include <memory>
#include <random>

#include "controller/Controller.h"
#include "entities/MainPlayer.h"
#include "scene/Scene.h"

#include "util/time_util.h"

#include "render/Model.h"
#include "world/World.h"

class SceneDefault : public Scene {

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
  double strafeAcceleration = 10;
  double strafeMaxSpeed = 5.4;
  double strafeDeceleration = 10;
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

  void updateWorld(double dt);

  void drawWorld();

  void drawCrosshair();

  void start();

  void triggerSpawn();

  void triggerOver();

public:

  std::shared_ptr<Controller> controller;
  std::shared_ptr<MainPlayer> player;
  std::shared_ptr<Model> targetModel;
  std::shared_ptr<Model> worldModel;

  SceneDefault();

  ~SceneDefault() override;

  void render(double dt) override;

  void open() override;

  void close() override;

  void resize(int width, int height) override;

  void handle(const UiEvent &event) override;

  bool hitSphere(const glm::vec3& rayOrigin, float pitch, float yaw,
               const glm::vec3& spherePos, float radius, float& tHit);

  bool isStarted() { return started; }

  bool isRunning() { return isStarted() && !over; };

};

#endif //SCENE_DEFAULT_H
