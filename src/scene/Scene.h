#ifndef SCENE_H
#define SCENE_H
#include "../ui/UiEvent.h"

class Scene {
public:
  Scene();

  void render();
  void open();
  void close();
  void resize(int width, int height);
  void handle(const UiEvent& event);

};

#endif //SCENE_H
