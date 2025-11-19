#include "ScreenResult.h"

#include "Application.h"
#include "Log.h"
#include "ScreenMain.h"
#include "ScreenSettings.h"

void ScreenResult::layout() {
  Ui::layout();

  msOpen = msCurrent();
}

// @formatter:off
std::unique_ptr<Ui> ScreenResult::createDrillProps() {
  std::vector<std::shared_ptr<Cell>> cells;
  for (auto drillProp : args.drillProps) {
    cells.push_back(Cell::abs(Column::make({
      Cell::abs(Label::make(drillProp.first, 0x808080ff, LabelAlign::LEFT, false), 18),
      Cell::abs(0),
      Cell::abs(Label::make(drillProp.second, 0xffffffff, LabelAlign::LEFT, false), 18),
    }), 200));
  }
  cells.push_back(Cell::rel());
  return Row::make(std::move(cells));
}
// @formatter:on

// @formatter:off
std::unique_ptr<Ui> ScreenResult::createMainStats() {
  std::vector<std::shared_ptr<Cell>> cells;
  for (auto drillProp : args.mainStats) {
    cells.push_back(Cell::abs(Label::make(drillProp.first, 0x808080ff, LabelAlign::LEFT, false), 18));
    cells.push_back(Cell::abs(0));
    cells.push_back(Cell::abs(Label::make(drillProp.second, 0xffffffff, LabelAlign::LEFT, true), 32));
    cells.push_back(Cell::abs(8));
  }
  cells.push_back(Cell::rel());
  return Column::make(std::move(cells));
}

// @formatter:off
ScreenResult::ScreenResult(ScreenResultArgs&& args) :args(args){
  std::vector<std::shared_ptr<Cell>> cells;
  cells.push_back(Cell::rel());

  // to drill property panel:
  cells.push_back(Cell::wrap(Row::make({
      Cell::rel(),
      Cell::abs(createDrillProps(), 500),
      Cell::rel(),
  })));
  cells.push_back(Cell::abs(20));

  // side stats panel
  cells.push_back(Cell::wrap(Row::make({
      Cell::rel(),
      Cell::abs(createMainStats(), 500),
      Cell::rel(),
  })));
  cells.push_back(Cell::abs(100));

  // buttons
  cells.push_back(Cell::abs(Row::make({
    Cell::rel(1),
    Cell::abs(Button::make("Restart", std::bind(&ScreenResult::actionRestart, this)), 150),
    Cell::rel(1)
  }), 30));
  cells.push_back(Cell::abs(10));
  cells.push_back(Cell::abs(Row::make({
    Cell::rel(1),
    Cell::abs(Button::make("Quit", std::bind(&ScreenResult::actionQuit, this)), 150),
    Cell::rel(1)
  }), 30));

  cells.push_back(Cell::rel());

  add(Column::make(std::move(cells)));

  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(1),
      Cell::rel(Row::make({
        Cell::rel(1),
        Cell::abs(Button::make("Settings", std::bind(&ScreenResult::actionSettings, this)), 120),
      }), 1),
    }), 30),
    Cell::rel(),
  }));
}
// @formatter:on

void ScreenResult::actionRestart() {
  Application::app.getScene()->replay();
  Application::app.setScreen(nullptr);
}

void ScreenResult::actionQuit() {
  Application::app.updateScene(nullptr);
  Application::app.setScreen(std::make_shared<ScreenMain>());
}

void ScreenResult::actionSettings() {
  auto &app = Application::app;
  auto screen = dynamic_pointer_cast<ScreenResult>(app.getScreen());
  if (!screen) {
    Log::warn("ScreenPause::actionSettings is miss-timed");
    return;
  }
  app.setScreen(std::make_shared<ScreenSettings>(screen)); // NOTE: PLACEMENT HERE IMPORTANT TO NOT DESTROY THIS
}

void ScreenResult::handle(UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON) {
    uint64_t openDurationMs = msCurrent() - msOpen;
    if (openDurationMs < 500) {
      event.claimed = true; // eat clicks after opening to avoid unwanted clicks
    }
  }

  auto &app = Application::app;
  if (event.type == UiEventType::KEY && event.down) {
    if (event.button == GLFW_KEY_Q) {
      app.later(std::bind(&ScreenResult::actionQuit, this));
    } else if (event.button == GLFW_KEY_R) {
      app.later(std::bind(&ScreenResult::actionRestart, this));
    } else if (event.button == GLFW_KEY_S) {
      app.later(std::bind(&ScreenResult::actionSettings, this));
    }
  }

  Ui::handle(event);
}
