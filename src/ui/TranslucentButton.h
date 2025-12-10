#ifndef TRANSLUCENT_BUTTON_H
#define TRANSLUCENT_BUTTON_H

#include <functional>

#include "Ui.h"

enum class TranslucentButtonAlign {
  LEFT, CENTER, RIGHT
};

class TranslucentButton : public Ui {
  std::string text;
  std::function<void()> listener = nullptr;
  TranslucentButtonAlign align = TranslucentButtonAlign::CENTER;

  double hoverDuration = 0.0;

public:
  static std::shared_ptr<TranslucentButton> make(
    const std::string &text,
    const std::function<void()> &listener = nullptr,
    TranslucentButtonAlign align = TranslucentButtonAlign::CENTER
  ) {
    auto r = std::make_shared<TranslucentButton>(text);
    r->align = align;
    r->listener = listener;
    return r;
  }

  explicit TranslucentButton(const std::string &text) :
    text(text) {}

  void render(double dt, const UiRenderParams &params) override;

  void layout() override;

  void onClicked();

  void handle(UiEvent &event) override;

  void setText(const std::string &text) { this->text = text; }
};


#endif //TRANSLUCENT_BUTTON_H
