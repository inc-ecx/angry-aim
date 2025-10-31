#include "Button.h"

#include "../Application.h"

void Button::layout() {
  hoverDuration = 0.0;
}

void Button::render(double dt, const UiRenderParams &params) {
  Application &app = Application::app;
  RenderUi &renderer = app.renderUi;
  RenderFont &fr = app.renderFont;

  double transitionDuration = 0.2;

  bool isHovered = hovered();
  hoverDuration = std::clamp(hoverDuration + (isHovered ? dt : -dt), 0.0, transitionDuration);

  int colorDefault = 0x505050ff;
  int colorHovered = 0x707070ff;
  int rgba = UiUtil::lerpColor(colorDefault, colorHovered, static_cast<float>(hoverDuration / transitionDuration));

  renderer.start();
  renderer.color(rgba);
  if (params.toBuffer)
    glBlendFunc(GL_ONE, GL_ZERO);
  renderer.rect(x, y, width, height, 8.0f, 1.0f);
  if (params.toBuffer)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  renderer.stop();

  fr.start();
  if (params.toBuffer)
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
  fr.renderText(
    text,
    round(x + (width - fr.width(text)) / 2 + 0.5f),
    round(y + (height - fr.height()) / 2 + 0.5f),
    0xffffffff
  );
  if (params.toBuffer)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  fr.stop();
}

void Button::onClicked() {
  if (listener) {
    std::function<void()> l = listener;
    Application::app.later(
      [l]() {
        l();
      }
    );
  }
}

void Button::handle(UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON && event.down && event.button == GLFW_MOUSE_BUTTON_LEFT) {
    if (hovered()) {
      onClicked();
    }
  }
}
