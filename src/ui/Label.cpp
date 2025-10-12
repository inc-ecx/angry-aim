#include "Label.h"

void Label::render() {
  Application &app = Application::app;
  RenderFont &fr = app.renderFont;

  fr.start();
  fr.renderText(text, round(x + (width - fr.width(text)) / 2 + 0.5f), round(y + (height - fr.height() / 2) + 0.5f), 0xffffffff);
  fr.stop();
}
