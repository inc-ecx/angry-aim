#include "ScreenDrill.h"
#include "ScreenMain.h"

#include "../Application.h"

// @formatter:off
ScreenDrill::ScreenDrill() {
  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(),
      Cell::abs(lblMainStat = Label::make(""), 200),
      Cell::rel(),
    }), 30),
    Cell::rel(),
  }));
}
// @formatter:on

void ScreenDrill::handle(UiEvent &event) {
  if (event.type == UiEventType::KEY && event.down && event.button == GLFW_KEY_ESCAPE) {
    Application::app.later([] {
      Application::app.setScreen(std::make_shared<ScreenMain>());
      Application::app.updateScene(nullptr);
    });
  }
  Ui::handle(event);
}
