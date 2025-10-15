#ifndef SCENE_DEFAULT_H
#define SCENE_DEFAULT_H

#include <memory>
#include <random>

#include "scene/Scene.h"

#include "util/time_util.h"

#include "render/Model.h"
#include "world/World.h"

class SceneDefault : public Scene {

  std::default_random_engine rng;

  int duration = 5;

  // spawning
  int spawnDelay = 200;
  float spawnMinDist = 10;
  float spawnMaxDist = 30;
  float spawnMinStrafe = 1;
  float spawnMaxStrafe = 10;
  int spawnQueue = 0;
  uint64_t spawnMsSpawn = 0; // timestamp by when a spawn can happen

  // strafing
  int strafeMinDurationMs = 150;
  int strafeMaxDurationMs = 500;
  double strafeAcceleration = 20;
  double strafeMaxSpeed = 5.4;
  double strafeDeceleration = 30;
  uint64_t strafeMsSwitch = 0; // timestamp by when the strafe direction needs to be switched

  // stats
  int statsHit = 0;

  // game state
  uint64_t msStarted = 0;
  bool started = false;
  bool done = false;

  void updateMovement(double dt);

  void updateWorld(double dt);

  void drawWorld();

  void drawCrosshair();

  void start();

  void triggerSpawn();

  void triggerDone();

public:
  std::shared_ptr<World> world;
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

  bool isRunning() { return isStarted() && !done; };

};

#endif //SCENE_DEFAULT_H
