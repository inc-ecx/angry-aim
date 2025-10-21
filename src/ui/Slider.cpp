#include "Slider.h"

#include "Application.h"

Slider::Slider(const std::function<void()> &changedListener) : changedListener(changedListener) {
}

void Slider::onSlide() {
  if (changedListener != nullptr) changedListener();
}

void Slider::handle(UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON) {
    if (event.button != GLFW_MOUSE_BUTTON_LEFT) {
      dragging = false;
    } else {
      dragging = hovered() && event.down;
    }
  }
}

void Slider::updateDrag(double dt) {
  if (!dragging) return;

  auto &app = Application::app;

  double bef = value;
  int sledX = static_cast<int>(x + floor(width - sledWidth) * value);
  double delta = (app.getMouseX() - sledWidth / 2.0 - sledX) / (width - sledWidth);

  bool creeping = glfwGetKey(app.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
  if (creeping) {
    delta *= creepSpeed * dt;
    if (delta > 0) {
      delta = pow(delta, 2) * 0.7f;
    } else {
      delta = -pow(delta, 2) * 0.7f;
    }
  }

  if (!std::isnan(delta)) {
    value += delta;
    if (value < 0) value = 0;
    if (value > 1) value = 1;
    if (bef != value) onSlide();
  }
}

void Slider::render(double dt) {
  auto &app = Application::app;
  auto &render = app.renderUi;

  bool isHovered = hovered();

  render.start();

  render.color(0x00000060);
  int railY = static_cast<int>(y + round((height - railHeight) / 2.0));
  render.rect(x, railY, width, railHeight);

  render.color(isHovered || dragging ? 0xffffffff : 0xc0c0c0ff);
  int sledX = static_cast<int>(x + floor(width - sledWidth) * value);
  render.rect(sledX, y, sledWidth, height);

  render.stop();

  updateDrag(dt);
}
