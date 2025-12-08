#include "Range.h"

#include <charconv>
#include <iomanip>
#include <sstream>
#include <valarray>

#include "Field.h"
#include "Row.h"
#include "Slider.h"

// @formatter:off
Range::Range(double value, double min, double max, double step,
             const std::optional<std::string> &minLabel,
             const std::optional<std::string> &maxLabel,
             const std::function<void(double fov)> &listenerChange)
:
  min(min), max(max), step(step), minLabel(minLabel), maxLabel(maxLabel), listenerChange(listenerChange)
{
  add(Row::make({
    Cell::rel(slider = Slider::make(std::bind(&Range::actionSlider, this))),
    Cell::abs(10),
    Cell::abs(field = Field::make("", std::bind(&Range::actionTextChanged, this), std::bind(&Range::actionTextSubmit, this)), 100),
  }));
  set(value);
}
// @formatter:on

void Range::onChange() {
  if (listenerChange) listenerChange(get());
}

void Range::actionSlider() {
  int decimalPlaces = -static_cast<int>(floor(std::log10(step)));
  if (decimalPlaces < 0) decimalPlaces = 0;

  double value = get();

  std::stringstream ss;
  ss << std::setprecision(decimalPlaces) << std::fixed << value;

  field->setSilently(ss.str());
  onChange();
}

void Range::actionTextChanged() {
  std::string text = field->text();

  double value;
  auto [ptr,ec] = std::from_chars(text.data(), text.data() + text.size(), value);
  if (ec != std::errc() || ptr != text.data() + text.size()) {
    // not a double.
    return;
  }

  if (value < min || value > max) {
    // outside of bounds
    return;
  }

  slider->setSilently(value, min, max, step);
  onChange();
}

void Range::actionTextSubmit() {}
