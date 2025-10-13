#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "../ui/UiEvent.h"
#include "../render/Model.h"

#include "../world/World.h"

class Scene {
public:
  Scene();

  ~Scene();

  std::shared_ptr<World> world;
  std::shared_ptr<Model> model;

  void render(double dt);
  void open();
  void close();
  void resize(int width, int height);
  void handle(const UiEvent& event);

};

#endif //SCENE_H
