#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

#include "../ui/Ui.h"
#include "../ui/Button.h"

class ScreenMain : public Ui {
public:
  ScreenMain() {
    children.push_back(std::make_unique<Button>());
  }

};

#endif //SCREEN_MAIN_H
