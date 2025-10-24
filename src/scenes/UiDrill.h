#ifndef SCREEN_DRILL_H
#define SCREEN_DRILL_H

#include "ui/components.h"

class UiDrill : public Ui {
public:
  std::shared_ptr<Label> lblMainStat;

  UiDrill();

  void handle(UiEvent &event) override;
};

#endif //SCREEN_DRILL_H
