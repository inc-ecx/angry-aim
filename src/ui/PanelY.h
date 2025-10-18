#ifndef SCROLL_COLUMN_H
#define SCROLL_COLUMN_H
#include <functional>

#include "Ui.h"

// The scroll bar control.
class BarY : public Ui {
  // bar properties
  std::function<void(float value)> listener;
  float value = 0.0f;

  // layout properties
  int sliderHeight = 0;

  // input state
  bool dragging = false;

  // animation state
  float fromValue = 0;
  float approachValue = 0;
  int durationMs = 0;
  uint64_t msApproached = 0;

  void onScroll();

public:
  explicit BarY(const std::function<void(float value)> &listener);

  void layout() override;

  void handle(UiEvent &event) override;

  void render(double dt) override;

  void setValue(float value);

  float getValue() { return value; }

  void approachValueOvercorrected(float value, int durationMs);
};

// The scroll panel item is the element added to the scroll panel container. It takes in the height it will have.
class ItemY : public Ui {
public:
  int itemHeight;

  ItemY(std::shared_ptr<Ui> &&child, int itemHeight)
    : Ui({std::move(child)}),
      itemHeight(itemHeight) {
  }

  explicit ItemY(int itemHeight)
    : itemHeight(itemHeight) {
  }

  static std::shared_ptr<ItemY> make(std::shared_ptr<Ui> &&child, int itemHeight) {
    return std::make_shared<ItemY>(std::move(child), itemHeight);
  }

  static std::shared_ptr<ItemY> make(int itemHeight) {
    return std::make_shared<ItemY>(itemHeight);
  }
};

// The Container is responsible for holding all items of the panel.
// It takes in the scroll position from the panel and lays the child items out accordingly.
class PanelYContainer : public Ui {
  // scroll state
  float value = 0;
  int yOff = 0;

public:
  explicit PanelYContainer(const std::vector<std::shared_ptr<ItemY> > &children) {
    for (auto child: children) this->children.push_back(child);
  }

  void layout() override;

  void render(double dt) override;

  int getScrollDistance();

  int getContentHeight();

  void onScrollChange(float value);
};

// The scroll panel. Holds the scrolling controls and the container for the item children.
// Children should only be added to the container.
class PanelY : public Ui {
  std::shared_ptr<PanelYContainer> _container;
  std::shared_ptr<BarY> bar;

  void actionScrollBar(float value);

public:
  explicit PanelY(const std::vector<std::shared_ptr<ItemY> > &children);

  static std::shared_ptr<PanelY> make(const std::vector<std::shared_ptr<ItemY> > &children) {
    return std::make_shared<PanelY>(children);
  }

  PanelYContainer &container() { return *_container; }
};

#endif //SCROLL_COLUMN_H
