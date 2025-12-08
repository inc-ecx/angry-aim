#ifndef SCREEN_RESULT_H
#define SCREEN_RESULT_H

#include "drill/model/Drill.h"
#include "ui/components.h"

struct ScreenResultArgs {
  Drill drill;
  std::vector<std::pair<std::string, std::string> > drillProps;
  std::vector<std::pair<std::string, std::string> > mainStats;
};

class ScreenResult : public Ui {
  std::unique_ptr<Ui> createDrillProps();

  std::unique_ptr<Ui> createMainStats();

  uint64_t msOpen = false;

  void actionRestart();

  void actionQuit();

  void actionSettings();

public:
  ScreenResultArgs args;

  explicit ScreenResult(ScreenResultArgs &&args);

  void handle(UiEvent &event) override;

  void layout() override;
};


#endif //SCREEN_RESULT_H
