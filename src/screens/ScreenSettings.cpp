#include "ScreenSettings.h"

ScreenSettings::ScreenSettings(const std::shared_ptr<Ui> &prev) : prev(prev)  {
  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(Row::make({
        Cell::abs(Button::make("Back", std::bind(actionBack, this)), 120),
        Cell::rel(1),
      }), 1),
      Cell::abs(Label::make("AngryAim"), 150),
      Cell::rel(1),
    }), 30),
    Cell::rel(),
  }));
}

void ScreenSettings::actionBack() {
  Application::app.setScreen(prev);
}
