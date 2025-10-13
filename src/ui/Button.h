#ifndef BUTTON_H
#define BUTTON_H
#include <functional>

#include "Ui.h"

class Button : public Ui {
  std::string text;
  std::function<void()> listener = nullptr;

  public:

  static std::shared_ptr<Button> make(const std::string& text, const std::function<void()> &listener = nullptr) {
    auto r = std::make_shared<Button>(text);
    r->listener = listener;
    return r;
  }

  explicit Button(const std::string &text) :text(text) {}

  void render(double dt) override;

  void onClicked();

  void handle(UiEvent &event) override;
};

#endif //BUTTON_H
