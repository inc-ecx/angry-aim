#include "ScreenMain.h"
#include "Application.h"
#include "DialogHelp.h"

#include "ScreenSettings.h"
#include "ScreenTest.h"
#include "drill/model/Drill.h"
#include "drill/model/ManagerDrill.h"

#include "scenes/SceneBackpack.h"
#include "drill_controllers/DrillStrafe.h"
#include "scenes/SceneDrill.h"
#include "state/State.h"
#include "ui/Image.h"
#include "ui/PanelY.h"

static std::string _sceneButtonText() {
  const Drill& drill = State::state.drill.currentDrill;
  return std::format("[{}]", drill.title);
}

// @formatter:off
std::shared_ptr<Ui> ScreenMain::constructDrillList() {
  std::vector<std::shared_ptr<ItemY> > items;
  for (auto &drill: ManagerDrill::inst.drills) {
    items.push_back(ItemY::make(Row::make({
      Cell::abs(8/*scrollbar width*/ + 5),
      Cell::rel(TranslucentButton::make(drill.title, [this, drill] {
        State::state.drill.currentDrill = drill;
        sceneButton->setText(_sceneButtonText());
        drillListContainer->clear();
      })),
      Cell::abs(5)
    }), 20));
    items.push_back(ItemY::make(10));
  }
  // items.push_back(ItemY::make(Label::make("Scene selector panel"), 25));
  // items.push_back(ItemY::make(10));
  return PanelY::make(items);
}
// @formatter:on

// @formatter:off
ScreenMain::ScreenMain() {
  auto &app = Application::app;

  add(Column::make({
    Cell::rel(1),
    Cell::abs(Row::make({
      Cell::rel(),
      Cell::abs(drillListContainer = std::make_shared<Ui>(), 400),
      Cell::rel()
    }), 100),
    Cell::abs(10),
    Cell::abs(Row::make({
      Cell::rel(1),
      Cell::abs(sceneButton = TranslucentButton::make(_sceneButtonText(), std::bind(&ScreenMain::actionSelectDrill, this)), 400),
      Cell::rel(1)
    }), 30),
    Cell::abs(30),
    Cell::abs(Row::make({
      Cell::rel(1),
      Cell::abs(Button::make("Play", std::bind(&ScreenMain::actionPlay, this)), 150),
      Cell::rel(1)
    }), 30),
    Cell::abs(10),
    Cell::abs(Row::make({
      Cell::rel(1),
      Cell::abs(Button::make("Exit", std::bind(&ScreenMain::actionExit, this)), 150),
      Cell::rel(1)
    }), 30),
    Cell::rel(1),
  }));

  add(Column::make({
    Cell::abs(20),
    Cell::abs(Row::make({
      Cell::rel(1),
      Cell::abs(Image::make(app.textures.logoLarge->id), 368),
      Cell::rel(1),
    }), 80),
    Cell::rel(),
  }));

  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(1),
      Cell::abs(Button::make("Settings", std::bind(&ScreenMain::actionSettings, this)), 120),
    }), 30),
    Cell::rel(),
  }));
}

void ScreenMain::actionSelectDrill() {
  if (drillListContainer->children.empty()) {
    drillListContainer->add(constructDrillList());
    drillListContainer->layout();
  }
}

void ScreenMain::actionPlay() {
  const Drill& drill = State::state.drill.currentDrill;
  Application::app.updateScene(std::make_shared<SceneDrill>(drill));
}

void ScreenMain::actionExit() {
  glfwSetWindowShouldClose(Application::app.window, GLFW_TRUE);
}

void ScreenMain::actionSettings() {
  Application::app.setScreen(std::make_shared<ScreenSettings>(Application::app.getScreen()));
}

void ScreenMain::handle(UiEvent &event) {
  if (event.type == UiEventType::KEY && event.down) {
    if (event.button == GLFW_KEY_F1)
      Application::app.setScreen(std::make_shared<DialogHelp>(Application::app.getScreen()));
    if (event.button == GLFW_KEY_SPACE)
      Application::app.later(std::bind(&ScreenMain::actionPlay, this));
    if (event.button == GLFW_KEY_TAB)
      Application::app.later(std::bind(&ScreenMain::actionSelectDrill, this));
    if (event.button == GLFW_KEY_1)
      Application::app.updateScene(std::make_shared<SceneBackpack>());
    if (event.button == GLFW_KEY_T)
      Application::app.setScreen(std::make_shared<ScreenTest>(Application::app.getScreen()));
    if (event.button == GLFW_KEY_S)
      Application::app.later(std::bind(&ScreenMain::actionSettings, this));
  }

  Ui::handle(event);
}
