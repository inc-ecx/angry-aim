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

void Ui::render() {
  renderChildren();
}

void Ui::doLayeredLayout() const {
  for (const auto &child: children) {
    child->setBounds(x, y, width, height);
  }
}

void Ui::layoutChildren() {
  for (const auto &child: children) {
    child->layout();
  }
}

void Ui::renderChildren() {
  for (auto itr = children.rbegin(); itr != children.rend(); ++itr) {
    (*itr)->render();
  }
}

[[nodiscard]] bool Ui::hovered() const {
  int x = Application::app.getMouseX(), y = Application::app.getMouseY();
  if (x >= static_cast<float>(this->x) &&
      x < static_cast<float>(this->x + width) &&
      y >= static_cast<float>(this->y) &&
      y < static_cast<float>(this->y + height))
    return true;
  return false;
}
