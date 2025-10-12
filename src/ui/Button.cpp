#include "Button.h"

void Button::render() {
  Application &app = Application::app;
  RenderUi &renderer = app.renderUi;
  RenderFont &fr = app.renderFont;

  renderer.start();

  renderer.color(0xffffff80);
  // renderer.rect(app.getMouseX(),app.getMouseY(), 100, 100);

  renderer.color(hovered() ? 0xffffff40 : 0xffffff20);
  renderer.rect(x,y, width, height);

  renderer.stop();

  fr.start();
  fr.renderText(text, round(x + (width - fr.width(text)) / 2 + 0.5f), round(y + (height - fr.height() / 2) + 0.5f), 0xffffffff);
  fr.stop();
}
