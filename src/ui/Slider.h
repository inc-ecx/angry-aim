#ifndef SLIDER_H
#define SLIDER_H
#include <algorithm>
#include <functional>

#include "Ui.h"

class Slider : public Ui {
  static constexpr double epsilon = 0.000'000'1;

  double value = 0.0;

  // element configuration
  int sledWidth = 8;
  int railHeight = 6;
  double creepSpeed = 2;

  // input state
  bool dragging = false;

  // listener
  std::function<void()> changedListener;

  // input handler
  void updateDrag(double dt);

public:
  static std::shared_ptr<Slider> make(std::function<void()> changedListener) {
    return std::make_shared<Slider>(changedListener);
  }

  explicit Slider(const std::function<void()> &changedListener);

  //
  // ui events
  //

  void onSlide();

  void render(double dt, const UiRenderParams& params) override;

  void handle(UiEvent &event) override;

  //
  // setters and getters
  //

  double getValue() {
    return value;
  }

  void setValue(double v) {
    this->value = std::clamp(v, 0.0, 1.0);
    onSlide();
  }

  void setValueSilently(double v) {
    this->value = std::clamp(v, 0.0, 1.0);
  }

  // note: this may not work probably, if min and max are in the negatives.
  double get(double min, double max, double step) {
    double v = min + value * (max - min);
    return round(v / step) * step;
  }

  void set(double v, double min, double max, double step) {
    setValue((v-min)/(max-min));
  }

  void setSilently(double v, double min, double max, double step) {
    setValueSilently((v-min)/(max-min));
  }
};

#endif //SLIDER_H
