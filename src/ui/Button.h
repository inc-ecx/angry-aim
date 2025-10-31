#ifndef BUTTON_H
#define BUTTON_H
#include <functional>

#include "Ui.h"

class Button : public Ui {
  std::string text;
  std::function<void()> listener = nullptr;

  double hoverDuration = 0.0;

public:
  static std::shared_ptr<Button> make(const std::string &text, const std::function<void()> &listener = nullptr) {
    auto r = std::make_shared<Button>(text);
    r->listener = listener;
    return r;
  }

  explicit Button(const std::string &text) : text(text) {
  }

  void render(double dt, const UiRenderParams& params) override;

  void layout() override;

  void onClicked();

  void handle(UiEvent &event) override;

  void setText(const std::string &text) { this->text = text; }
};

#endif //BUTTON_H
