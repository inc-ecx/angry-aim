#ifndef RANGE_H
#define RANGE_H

#include "Field.h"
#include "Slider.h"
#include "Ui.h"

class Range : public Ui {
  double min, max, step;
  std::shared_ptr<Slider> slider;
  std::shared_ptr<Field> field;
  std::function<void(double fov)> listenerChange;

public:
  Range(double value, double min, double max, double step, const std::function<void(double fov)> &listenerChange);

  static std::shared_ptr<Range> make(double value, double min, double max, double step, std::function<void(double fov)> listenerChange) {
    return std::make_shared<Range>(value, min, max, step, listenerChange);
  }

  //
  // ui events
  //

  void onChange();

  void actionSlider();

  void actionTextChanged();

  void actionTextSubmit();

  //
  // accessors
  //

  double get() {
    return slider->get(min, max, step);
  }

  void set(double value) {
    slider->set(value, min, max, step);
  }
};

#endif //RANGE_H
