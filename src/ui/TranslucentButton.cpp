#include "TranslucentButton.h"

#include "../Application.h"

void TranslucentButton::layout() {
  hoverDuration = 0.0;
}

void TranslucentButton::render(double dt, const UiRenderParams &params) {
  Application &app = Application::app;
  RenderFont &fr = app.renderFont;

  double transitionDuration = 0.2;

  bool isHovered = hovered();
  hoverDuration = std::clamp(hoverDuration + (isHovered ? dt : -dt), 0.0, transitionDuration);

  int colorDefault = 0xc0c0c0ff;
  int colorHovered = 0xffffffff;
  int rgba = UiUtil::lerpColor(colorDefault, colorHovered, static_cast<float>(hoverDuration / transitionDuration));

  fr.start();
  if (params.toBuffer)
    glBlendFunc(GL_ONE, GL_ZERO);

  fr.renderText(
    text,
    round(x + (width - fr.width(text)) / 2 + 0.5f),
    round(y + (height - fr.height()) / 2 + 0.5f),
    rgba
  );
  if (params.toBuffer)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  fr.stop();
}

void TranslucentButton::onClicked() {
  if (listener) {
    std::function<void()> l = listener;
    Application::app.later(
      [l]() {
        l();
      }
    );
  }
}

void TranslucentButton::handle(UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON && event.down && event.button == GLFW_MOUSE_BUTTON_LEFT && !event.claimed) {
    if (hovered()) {
      onClicked();
    }
  }
}
