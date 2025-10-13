#ifndef SCENE_H
#define SCENE_H

#include "../ui/UiEvent.h"

class Scene {
public:
  Scene() {}

  virtual ~Scene() {}

  virtual void render(double dt) = 0;
  virtual void open() = 0;
  virtual void close() = 0;
  virtual void resize(int width, int height) = 0;
  virtual void handle(const UiEvent& event) = 0;

};

#endif //SCENE_H
