#ifndef SCREEN_RESULT_H
#define SCREEN_RESULT_H

#include "ui/components.h"

class ScreenResult : public Ui {

public:
  explicit ScreenResult(const std::vector<std::string> &results);

  void handle(UiEvent &event) override;

  void render(double dt) override;

  void handlePlay();
};



#endif //SCREEN_RESULT_H
