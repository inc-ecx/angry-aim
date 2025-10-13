#include "Button.h"

#include "../Application.h"

void Button::render(double dt) {
  Application &app = Application::app;
  RenderUi &renderer = app.renderUi;
  RenderFont &fr = app.renderFont;

  renderer.start();
  renderer.color(hovered() ? 0xffffff40 : 0xffffff20);
  renderer.rect(x, y, width, height);
  renderer.stop();

  fr.start();
  fr.renderText(text, round(x + (width - fr.width(text)) / 2 + 0.5f), round(y - fr.height() + (height - fr.height() / 2) + 0.5f),
                0xffffffff);
  fr.stop();
}

void Button::onClicked() {
  if (listener) {
    std::function<void()> l = listener;
    Application::app.later([l]() {
      l();
    });
  }
}

void Button::handle(UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON && event.down && event.button == GLFW_MOUSE_BUTTON_LEFT) {
    if (hovered()) {
      onClicked();
    }
  }
}
