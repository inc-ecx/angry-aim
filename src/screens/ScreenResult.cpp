#include "ScreenResult.h"

#include "Application.h"
#include "ScreenMain.h"

ScreenResult::ScreenResult(const std::vector<std::string> &results) {
  std::vector<std::shared_ptr<Cell>> cells;
  cells.push_back(Cell::rel());
  for (auto result : results) {
    cells.push_back(Cell::abs(Row::make({
      Cell::rel(),
      Cell::abs(Label::make(result), 200),
      Cell::rel(),
    }), 30));
  }
  cells.push_back(Cell::abs(60));
  cells.push_back(Cell::abs(Row::make({
      Cell::rel(),
      // Cell::abs(Button::make("Play", std::bind(&ScreenResult::handlePlay, this)), 200),
      Cell::abs(Label::make("(Space) Replay", 0xffffff80), 200),
      Cell::rel(),
  }), 30));
  cells.push_back(Cell::abs(Row::make({
      Cell::rel(),
      Cell::abs(Label::make("(Esc) Main Menu", 0xffffff80), 200),
      Cell::rel(),
    }), 30));
  cells.push_back(Cell::rel());

  add(Column::make({
    Cell::abs(Label::make("Default Results"), 30),
    Cell::rel(),
  }));
  add(Column::make(std::move(cells)));
}

void ScreenResult::render(double dt) {
  auto &app = Application::app;
  auto &render = app.renderUi;
  render.start();
  render.color(0x00000080);
  render.rect(x,y,width,height);
  render.stop();

  Ui::render(dt);
}

void ScreenResult::handlePlay() {
  Application::app.getScene()->replay();
  Application::app.setScreen(nullptr);
}

void ScreenResult::handle(UiEvent &event) {
  if (event.type == UiEventType::KEY && event.down && event.button == GLFW_KEY_ESCAPE) {
    Application::app.later([] {
      Application::app.updateScene(nullptr);
      Application::app.setScreen(std::make_shared<ScreenMain>());
    });
  }

  if (event.type == UiEventType::KEY && event.down && (event.button == GLFW_KEY_SPACE)) {
    Application::app.later(std::bind(&ScreenResult::handlePlay, this));
  }

  Ui::handle(event);
}
