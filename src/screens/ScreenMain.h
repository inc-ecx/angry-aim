#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

#include "../ui/components.h"
#include "drill/model/Drill.h"

class ScreenMain : public Ui {
  std::shared_ptr<Ui> drillListContainer;
  std::shared_ptr<TranslucentButton> sceneButton;
  std::shared_ptr<Field> searchField;

  std::shared_ptr<Ui> constructDrillList();

  std::shared_ptr<Ui> constructDrillSelector();

  void onSearchChanged();

  void onSearchSubmit();

  bool isSearchCharAllowed(char c);

  void actionSelectDrill();

  void actionPlay();

  void actionExit();

  void actionSettings();

public:
  ScreenMain();

  void handle(UiEvent &event) override;
};

#endif //SCREEN_MAIN_H
