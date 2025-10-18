#ifndef SCREEN_DRILL_H
#define SCREEN_DRILL_H

#include "ui/components.h"

class ScreenDrill : public Ui {
public:
  std::shared_ptr<Label> lblMainStat;

  ScreenDrill();

  void handle(UiEvent &event) override;
};

#endif //SCREEN_DRILL_H
