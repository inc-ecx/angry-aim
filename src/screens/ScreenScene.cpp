#include "ScreenScene.h"

#include "ScreenMain.h"

// @formatter:off
ScreenScene::ScreenScene() {
  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(),
      Cell::abs(Label::make("Time: 1:00"), 200),
      Cell::rel(),
    }), 30),
    Cell::rel(),
  }));
}
// @formatter:on

void ScreenScene::handle(UiEvent &event) {
  if (event.type == UiEventType::KEY && event.down && event.button == GLFW_KEY_ESCAPE) {
    Application::app.later([] {
      Application::app.setScreen(std::make_shared<ScreenMain>());
      Application::app.setScene(nullptr);
    });
  }

  Ui::handle(event);
}
