#include "Label.h"

#include "../Application.h"

void Label::render(double dt) {
  auto &app = Application::app;
  auto &fr = app.renderFont;

  fr.start();
  float tw = static_cast<float>(fr.width(text));
  float tx;
  if (align == LabelAlign::LEFT) {
    tx = static_cast<float>(x);
  } else if (align == LabelAlign::RIGHT) {
    tx = x + width - tw;
  } else {
    tx = round(x + (width - tw) / 2 + 0.5f);
  }
  float ty = round(y + (height - fr.height()) / 2 + 0.5f);
  fr.renderText(text, tx, ty, rgba);

  fr.stop();
}
