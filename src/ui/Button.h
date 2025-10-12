#ifndef BUTTON_H
#define BUTTON_H
#include "Ui.h"

class Button : public Ui {
  std::string text;

  public:

  static std::shared_ptr<Button> make(const std::string text) {
    return std::make_shared<Button>(text);
  }

  explicit Button(const std::string &text) :text(text) {}

  void render() override;
};

#endif //BUTTON_H
