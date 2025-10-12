#include "Button.h"
#include <iostream>

void Button::render() {
  Application &app = Application::app;
  RenderUi &renderer = app.renderUi;
  renderer.rect(app.getMouseX(),app.getMouseY(), 10, 10);
}
