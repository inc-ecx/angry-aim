#ifndef SCENE_DEFAULT_H
#define SCENE_DEFAULT_H

#include "Scene.h"

#include <memory>

#include "../render/Model.h"
#include "../world/World.h"

class SceneDefault : public Scene {
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
