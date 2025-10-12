#ifndef BUTTON_H
#define BUTTON_H
#include "Ui.h"

class Button : public Ui {
public:

  static std::shared_ptr<Button> make() {
    return std::make_shared<Button>();
  }

  Button() {}

  void render() override;
};

#endif //BUTTON_H
