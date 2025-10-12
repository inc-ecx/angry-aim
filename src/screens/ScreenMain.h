#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

#include "../ui/components.h"

class ScreenMain : public Ui {
public:
  ScreenMain();

  void actionScene();
  void actionPlay();
  void actionSettings();
};

#endif //SCREEN_MAIN_H
