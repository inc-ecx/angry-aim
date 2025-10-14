#ifndef SCENE_DEFAULT_H
#define SCENE_DEFAULT_H

#include <memory>
#include <random>

#include "scene/Scene.h"

#include "util/time_util.h"

#include "render/Model.h"
#include "world/World.h"

class SceneDefault : public Scene {

  // timestamp by when the strafe direction needs to be switched
  int strafeMinDurationMs = 230;
  int strafeMaxDurationMs = 1300;
  double strafeAcceleration = 30;
  double strafeMaxSpeed = 6;
  double strafeDeceleration = 40;
  std::default_random_engine strafeRandom;
  uint64_t strafeMsSwitch = 0;

  void updateMovement(double dt);

  void updateWorld(double dt);

  void drawWorld();

  void drawCrosshair();

public:
  std::shared_ptr<World> world;
  std::shared_ptr<Model> model;

  SceneDefault();

  ~SceneDefault() override;

  void render(double dt) override;

  void open() override;

  void close() override;

  void resize(int width, int height) override;

  void handle(const UiEvent &event) override;
};

#endif //SCENE_DEFAULT_H
