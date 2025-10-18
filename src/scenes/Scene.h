#ifndef SCENE_H
#define SCENE_H

#include "ui/Ui.h"

class Scene {
public:
  Scene() {}

  virtual ~Scene() {}

  virtual void replay() {}
  virtual void open() {}
  virtual void close() {}
  virtual void resize(int width, int height) {}

  virtual void render(double dt) = 0;
  virtual void handle(const UiEvent& event) = 0;

};

#endif //SCENE_H
