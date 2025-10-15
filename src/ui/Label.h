#ifndef LABEL_H
#define LABEL_H

#include "Ui.h"

class Label : public Ui {
  std::string text;

public:
  static std::shared_ptr<Label> make(const std::string &text) {
    return std::make_shared<Label>(text);
  }

  explicit Label(const std::string &text) : text(text) {
  }

  void render(double dt) override;

  void setText(const std::string &text) { this->text = text; }
};

#endif //LABEL_H
