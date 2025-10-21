#include "Ui.h"

#include "../Application.h"

void Ui::handleChildren(UiEvent &event) {
  for (const auto &child: children) {
    child->handle(event);
  }
}

void Ui::handle(UiEvent &event) {
  handleChildren(event);
}

void Ui::layout() {
  doLayeredLayout();
  layoutChildren();
}

void Ui::render(double dt) {
  renderChildren(dt);
}

void Ui::doLayeredLayout() const {
  for (const auto &child: children) {
    child->setBounds(x, y, width, height);
    child->setViewport(vpx1, vpy1, vpx2, vpy2);
  }
}

void Ui::layoutChildren() {
  for (const auto &child: children) {
    child->layout();
  }
}

void Ui::renderChildren(double dt) {
  for (auto itr = children.rbegin(); itr != children.rend(); ++itr) {
    (*itr)->render(dt);
  }
}

[[nodiscard]] bool Ui::hovered() const {
  int x = Application::app.getMouseX(), y = Application::app.getMouseY();

  if (x >= static_cast<float>(this->x) &&
      x < static_cast<float>(this->x + width) &&
      y >= static_cast<float>(this->y) &&
      y < static_cast<float>(this->y + height) &&
      x >= static_cast<float>(this->vpx1) &&
      x < static_cast<float>(this->vpx2) &&
      y >= static_cast<float>(this->vpy1) &&
      y < static_cast<float>(this->vpy2))
    return true;

  return false;
}
