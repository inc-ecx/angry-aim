#include "ScreenSettings.h"

#include "../Application.h"

// @formatter:off
ScreenSettings::ScreenSettings(const std::shared_ptr<Ui> &prev) : prev(prev)  {
  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(Row::make({
        Cell::abs(Button::make("Back", std::bind(&ScreenSettings::actionBack, this)), 120),
        Cell::rel(1),
      }), 1),
      Cell::abs(Label::make("AngryAim"), 150),
      Cell::rel(1),
    }), 30),
    Cell::rel(),
  }));
}

void ScreenSettings::handle(UiEvent &event) {
  if (event.type == UiEventType::KEY && event.down && event.button == GLFW_KEY_ESCAPE) {
    std::shared_ptr<Ui> prev = this->prev;
    Application::app.later([prev] {
      Application::app.setScreen(prev);
    });
  }

  Ui::handle(event);
}

// @formatter:on

void ScreenSettings::actionBack() {
  Application::app.setScreen(prev);
}
