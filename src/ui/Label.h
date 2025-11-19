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
  bool isStats;

public:
  Label(const std::string &text, int rgba, LabelAlign align, bool isStats) :
    text(text),
    rgba(rgba),
    align(align),
    isStats(isStats) {}

  static std::shared_ptr<Label> make(const std::string &text) {
    return std::make_shared<Label>(text, 0xffffffff, LabelAlign::CENTER, false);
  }

  static std::shared_ptr<Label> make(const std::string &text, LabelAlign align) {
    return std::make_shared<Label>(text, 0xffffffff, align, false);
  }

  static std::shared_ptr<Label> make(const std::string &text, int rgba) {
    return std::make_shared<Label>(text, rgba, LabelAlign::CENTER, false);
  }

  static std::shared_ptr<Label> make(const std::string &text, int rgba, LabelAlign align) {
    return std::make_shared<Label>(text, rgba, align, false);
  }

  static std::shared_ptr<Label> make(const std::string &text, int rgba, LabelAlign align, bool isStats) {
    return std::make_shared<Label>(text, rgba, align, isStats);
  }

  void render(double dt, const UiRenderParams &params) override;

  void setText(const std::string &text) { this->text = text; }

  void setRgba(int rgba) { this->rgba = rgba; }
};

#endif //LABEL_H
