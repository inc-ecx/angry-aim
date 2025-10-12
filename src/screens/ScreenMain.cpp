#include "ScreenMain.h"

// @formatter:off
ScreenMain::ScreenMain() {
  add(Row::make({
    Cell::rel(1),
    Cell::abs(Column::make({
      Cell::rel(1),
      Cell::abs(Button::make("[Default]"), 30),
      Cell::abs(10),
      Cell::abs(Button::make("Play"), 30),
      Cell::rel(1),
    }), 400),
    Cell::rel(1),
  }));
}
// @formatter:on
