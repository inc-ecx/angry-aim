#ifndef UI_H
#define UI_H

#include <limits>
#include <memory>
#include <vector>
#include <string>

#include "UiEvent.h"
#include "UiUtil.h"
#include "UiRenderParams.h"

// the basic building block of any ui element.
class Ui {
public:
  std::vector<std::shared_ptr<Ui> > children;

  int x = std::numeric_limits<int>::min(),
      y = std::numeric_limits<int>::min(),
      width = std::numeric_limits<int>::min(),
      height = std::numeric_limits<int>::min();

  int vpx1 = std::numeric_limits<int>::min(),
      vpy1 = std::numeric_limits<int>::min(),
      vpx2 = std::numeric_limits<int>::max(),
      vpy2 = std::numeric_limits<int>::max();

  static std::shared_ptr<Ui> make() {
    return std::make_shared<Ui>();
  }

  Ui() = default;

  virtual ~Ui() = default;

  void handleChildren(UiEvent &event);

  virtual void handle(UiEvent &event);

  Ui(const Ui &other) = delete;

  Ui &operator=(const Ui &other) = delete;

  Ui(Ui &&other) = delete;

  Ui &operator=(Ui &&other) = delete;

  explicit Ui(std::vector<std::shared_ptr<Ui> > &&children)
    : children(children) {
  }

  void add(std::shared_ptr<Ui> &&child) {
    children.push_back(std::move(child));
  }

  void clear() {
    children.clear();
  }

  // recalculates bounds of this element and its children. order depends on ui element.
  // setBounds was used before to set the maximum bounds possible
  virtual void layout();

  // renders this element using the ui renderer
  virtual void render(double dt, const UiRenderParams& params);

  // sets dimensions of this ui element
  void setBounds(const int x, const int y, const int width, const int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
  }

  // used by container ui elements looking for their children to give a wanted size dimension.
  // note: width and height must be zero-initialized. hasWidth and hasHeight must be false-initialized.
  virtual void queryWrap(int &width, int &height, bool &hasWidth, bool &hasHeight);

  void queryWrapChildren(int &width, int &height, bool &hasWidth, bool &hasHeight);

  // sets the viewport of this element. used by scroll panels.
  void setViewport(const int x1, const int y1, const int x2, const int y2) {
    this->vpx1 = x1;
    this->vpy1 = y1;
    this->vpx2 = x2;
    this->vpy2 = y2;
  }

  // gives every child the same size as this element
  void doLayeredLayout() const;

  // calls layout for every child
  void layoutChildren();

  // calls render for every child
  void renderChildren(double dt, const UiRenderParams &params);

  // return true, if the mouse hovers this element
  bool hovered() const;
};


#endif //UI_H
