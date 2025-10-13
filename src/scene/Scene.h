#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "../ui/UiEvent.h"
#include "../render/Model.h"

class Scene {
public:
  Scene();

  std::shared_ptr<Model> model;

  void render();
  void open();
  void close();
  void resize(int width, int height);
  void handle(const UiEvent& event);

};

#endif //SCENE_H
