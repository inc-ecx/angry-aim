#include "Label.h"

#include "../Application.h"

void Label::render(double dt) {
  Application &app = Application::app;
  RenderFont &fr = app.renderFont;

  fr.start();
  fr.renderText(text, round(x + (width - fr.width(text)) / 2 + 0.5f), round(y - fr.height() + (height - fr.height() / 2) + 0.5f), 0xffffffff);
  fr.stop();
}
