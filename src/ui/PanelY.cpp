#include "PanelY.h"

#include "Application.h"
#include "Row.h"
#include "util/math_util.h"

//
// Bar Class
//

BarY::BarY(const std::function<void(float value)> &listener) : listener(listener) {
}

void BarY::layout() {
  sliderHeight = static_cast<int>(height * 0.2f);
}

// scroll column bar
void BarY::handle(UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON) {
    dragging = hovered() && event.down && event.button == GLFW_MOUSE_BUTTON_LEFT;
  }
}

void BarY::render(double dt) {
  auto &app = Application::app;
  auto &render = app.renderUi;

  // render code

  int barY = static_cast<int>(floor((height - sliderHeight) * value));

  render.start();

  render.color(0x30000000);
  render.rect(x, y, width, height);

  render.color(hovered() || dragging ? 0xc0ffffff : 0xc0d0d0d0);
  render.rect(x, y + barY, width, sliderHeight);

  render.stop();

  // update code
  int64_t approachPast = static_cast<int64_t>(msApproached - msCurrent());
  if (msApproached != 0 && approachPast > 0) {
    float x = static_cast<float>(approachPast) / durationMs;
    value = fromValue + smoothStep(1, 0, x) * (approachValue - fromValue);
    onScroll();
  }

  if (dragging) {
    int mouseY = app.getMouseY();

    msApproached = 0;
    float bef = value;
    value = static_cast<float>(mouseY - y - sliderHeight / 2) / (height - sliderHeight);

    if (value < 0) value = 0;
    if (value > 1) value = 1;
    if (bef != value) {
      onScroll();
    }
  }
}

void BarY::onScroll() {
  if (listener) listener(value);
}

void BarY::setValue(float value) {
  this->value = std::clamp(value, 0.0f, 1.0f);
  onScroll();
}

void BarY::approachValueOvercorrected(float value, int durationMs) {
  msApproached = msCurrent() + durationMs;
  this->durationMs = durationMs;
  fromValue = this->value;
  int approachPast = static_cast<int>(msApproached - msCurrent());
  if (approachPast > 0) {
    // overcorrection
    float x = static_cast<float>(approachPast) / durationMs;
    value += x * (approachValue - this->value);
  }
  value = std::clamp(value, 0.0f, 1.0f);
  approachValue = value;
}

//
// Panel Container Class
//

void PanelYContainer::layout() {
  int currentY = y + yOff;
  for (auto &child : children) {
    auto item = std::dynamic_pointer_cast<ItemY>(child);
    if (item == nullptr) continue;
    item->setBounds(x, currentY, width, item->itemHeight);
    currentY += item->itemHeight;
  }
  layoutChildren();
}

void PanelYContainer::render(double dt) {
  auto &app = Application::app;

  app.pushScissors(x,y,width,height);
  renderChildren(dt);
  app.popScissors();
}

void PanelYContainer::onScrollChange(float value) {
  this->value = value;

  yOff = static_cast<int>(-this->value * getScrollDistance());

  layout();
}

int PanelYContainer::getScrollDistance() {
  int extraGap = 10;
  return std::max(getContentHeight() - height + extraGap, 0);
}

int PanelYContainer::getContentHeight() {
  int h = 0;
  for (auto &child : children) {
    auto item = std::dynamic_pointer_cast<ItemY>(child);
    if (item == nullptr) continue;
    h += item->itemHeight;
  }
  return h;
}


//
// Panel Class
//

PanelY::PanelY(const std::vector<std::shared_ptr<ItemY>>& children) {
  this->children.push_back(Row::make({
    Cell::rel(_container = std::make_unique<PanelYContainer>(children)),
    Cell::abs(bar = std::make_shared<BarY>(std::bind(&PanelY::actionScrollBar, this, std::placeholders::_1)), 8)
  }));
}

void PanelY::actionScrollBar(float value) {
  _container->onScrollChange(value);
}