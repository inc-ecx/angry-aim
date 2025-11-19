#include "Label.h"

#include "../Application.h"

void Label::render(double dt, const UiRenderParams &params) {
  auto &app = Application::app;
  auto &fr = isStats ? app.renderFontStats : app.renderFont;

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

  if (params.toBuffer)
    glBlendFunc(GL_ONE, GL_ZERO);
  fr.renderText(text, tx, ty, rgba);
  if (params.toBuffer)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  fr.stop();
}
