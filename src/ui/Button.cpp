#include "Button.h"
#include <iostream>

void Button::render() {
  Application &app = Application::app;
  RenderUi &renderer = app.renderUi;

  renderer.color(0xffffff80);
  // renderer.rect(app.getMouseX(),app.getMouseY(), 100, 100);

  renderer.color(hovered() ? 0xffffff40 : 0xffffff20);
  renderer.rect(x,y, width, height);
}

void Button::layout() {
  std::cout << "button layout: " << x << " " << y;
}
