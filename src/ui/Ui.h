#ifndef UI_H
#define UI_H

#include <limits>
#include <memory>
#include <vector>
#include <string>

#include "UiEvent.h"

// the basic building block of any ui element.
class Ui {
public:
  // std::weak_ptr<Ui> parent;
  std::vector<std::shared_ptr<Ui> > children;

  int x = std::numeric_limits<int>::min(),
      y = std::numeric_limits<int>::min(),
      width = std::numeric_limits<int>::min(),
      height = std::numeric_limits<int>::min();

  static std::shared_ptr<Ui> make() {
    return std::make_shared<Ui>();
  }

  Ui() = default;

  virtual ~Ui() = default;

  void handleChildren(UiEvent &event);

  virtual void handle(UiEvent &event);

  Ui(const Ui &other) = delete;

  Ui &operator=(const Ui &other) = delete;

  Ui(Ui &&other) noexcept
    :
    // : parent(std::move(other.parent)),
    children(std::move(other.children)),
    x(other.x),
    y(other.y),
    width(other.width),
    height(other.height) {
  }

  Ui &operator=(Ui &&other) noexcept {
    if (this == &other)
      return *this;
    // parent = std::move(other.parent);
    children = std::move(other.children);
    x = other.x;
    y = other.y;
    width = other.width;
    height = other.height;
    return *this;
  }

  explicit Ui(std::vector<std::shared_ptr<Ui> > &&children)
    : children(children) {
  }

  void add(std::shared_ptr<Ui> &&child) {
    children.push_back(std::move(child));
  }

  // recalculates bounds of this element and its children. order depends on ui element.
  // setBounds was used before to set the maximum bounds possible
  virtual void layout();

  // renders this element using the ui renderer
  virtual void render();

  void setBounds(const int x, const int y, const int width, const int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
  }

  // gives every child the same size as this element
  void doLayeredLayout() const;

  // calls layout for every child
  void layoutChildren();

  // calls render for every child
  void renderChildren();

  // return true, if the mouse hovers this element
  bool hovered() const;
};


#endif //UI_H
