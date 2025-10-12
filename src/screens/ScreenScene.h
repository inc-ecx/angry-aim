#ifndef SCREEN_SCENE_H
#define SCREEN_SCENE_H

#include "../ui/components.h"

class ScreenScene : public Ui {
public:
  ScreenScene();

  void handle(UiEvent &event) override;
};

#endif //SCREEN_SCENE_H
