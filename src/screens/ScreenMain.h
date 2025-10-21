#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

#include "../ui/components.h"
#include "drill/model/Drill.h"

class ScreenMain : public Ui {
  std::shared_ptr<Ui> drillListContainer;
  std::shared_ptr<Button> sceneButton;

  std::shared_ptr<Ui> constructDrillList();

  void actionSelectDrill();

  void actionPlay();

  void actionSettings();

public:
  ScreenMain();

  void handle(UiEvent &event) override;
};

#endif //SCREEN_MAIN_H
