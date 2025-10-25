#ifndef LABEL_H
#define LABEL_H

#include "Ui.h"

enum class LabelAlign {
  LEFT, CENTER, RIGHT
};

class Label : public Ui {
  std::string text;
  int rgba = 0xffffffff;
  LabelAlign align;

public:
  Label(const std::string &text, int rgba, LabelAlign align) :
    text(text),
    rgba(rgba),
    align(align) {}

  static std::shared_ptr<Label> make(const std::string &text) {
    return std::make_shared<Label>(text, 0xffffffff, LabelAlign::CENTER);
  }

  static std::shared_ptr<Label> make(const std::string &text, LabelAlign align) {
    return std::make_shared<Label>(text, 0xffffffff, align);
  }

  static std::shared_ptr<Label> make(const std::string &text, int rgba) {
    return std::make_shared<Label>(text, rgba, LabelAlign::CENTER);
  }

  static std::shared_ptr<Label> make(const std::string &text, int rgba, LabelAlign align) {
    return std::make_shared<Label>(text, rgba, align);
  }

  void render(double dt) override;

  void setText(const std::string &text) { this->text = text; }

  void setRgba(int rgba) { this->rgba = rgba; }
};

#endif //LABEL_H
