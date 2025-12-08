#ifndef RANGE_H
#define RANGE_H

#include <optional>

#include "Field.h"
#include "Slider.h"
#include "Ui.h"

class Range : public Ui {
  double min, max, step;
  std::shared_ptr<Slider> slider;
  std::shared_ptr<Field> field;
  std::optional<std::string> minLabel;
  std::optional<std::string> maxLabel;
  std::function<void(double fov)> listenerChange;

public:
  Range(
    double value, double min, double max, double step,
    const std::optional<std::string> &minLabel,
    const std::optional<std::string> &maxLabel,
    const std::function<void(double fov)> &listenerChange
  );

  static std::shared_ptr<Range> make(
    double value, double min, double max, double step, std::function<void(double fov)> listenerChange
  ) {
    return std::make_shared<Range>(value, min, max, step, std::nullopt, std::nullopt, listenerChange);
  }

  // static std::shared_ptr<Range> make(
  //   double value, double min, double max, double step,
  //   std::optional<std::string> minLabel,
  //   std::optional<std::string> maxLabel,
  //   std::function<void(double fov)> listenerChange
  // ) {
  //   return std::make_shared<Range>(value, min, max, step, minLabel, maxLabel, listenerChange);
  // }

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
