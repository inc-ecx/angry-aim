#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

#include "../ui/Ui.h"
#include "../ui/Button.h"
#include "../ui/Column.h"

class ScreenMain : public Ui {
public:
  ScreenMain() {
    children.push_back(Column::make({
      Cell::abs(std::make_shared<Button>(), 20),
      Cell::abs(std::make_shared<Button>(), 20)
    }));
  }

};

#endif //SCREEN_MAIN_H
