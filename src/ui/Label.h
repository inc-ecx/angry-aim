#ifndef LABEL_H
#define LABEL_H

#include "Ui.h"

class Label : public Ui {
  std::string text;
  int rgba = 0xffffffff;

public:
  Label(const std::string &text, int rgba)
    : text(text),
      rgba(rgba) {
  }

  static std::shared_ptr<Label> make(const std::string &text) {
    return std::make_shared<Label>(text, 0xffffffff);
  }
  static std::shared_ptr<Label> make(const std::string &text, int rgba) {
    return std::make_shared<Label>(text, rgba);
  }

  void render(double dt) override;

  void setText(const std::string &text) { this->text = text; }

  void setRgba(int rgba) { this->rgba = rgba; }
};

#endif //LABEL_H
