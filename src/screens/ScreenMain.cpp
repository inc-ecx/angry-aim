#include "ScreenMain.h"
#include "Application.h"

#include "ScreenSettings.h"
#include "drill/model/Drill.h"
#include "drill/model/DrillManager.h"

#include "scenes/SceneBackpack.h"
#include "drill_controllers/DrillSimple.h"
#include "drill_controllers/DrillDefault.h"
#include "drill_controllers/FactoryDrill.h"
#include "scenes/SceneDrill.h"
#include "ui/PanelY.h"

std::shared_ptr<Ui> ScreenMain::constructDrillList() {
  std::vector<std::shared_ptr<ItemY> > items;
  for (auto &drill: DrillManager::inst.drills) {
    items.push_back(ItemY::make(Row::make({
                                  Cell::abs(8/*scrollbar width*/ + 5),
                                  Cell::rel(Button::make(drill.name, [this, drill] {
                                    this->drill = drill;
                                    sceneButton->setText(std::format("[{}]", drill.name));
                                    drillListContainer->clear();
                                  })),
                                  Cell::abs(5)
                                }), 25));
    items.push_back(ItemY::make(10));
  }
  // items.push_back(ItemY::make(Label::make("Scene selector panel"), 25));
  // items.push_back(ItemY::make(10));
  return PanelY::make(items);
}

// @formatter:off
ScreenMain::ScreenMain() : drill(DrillManager::inst.def) {
  add(Row::make({
    Cell::rel(1),
    Cell::abs(Column::make({
      Cell::rel(1),
      Cell::abs(drillListContainer = std::make_shared<Ui>(), 100),
      Cell::abs(10),
      Cell::abs(Row::make({
        Cell::rel(1),
        Cell::abs(sceneButton = Button::make("[Default]", std::bind(&ScreenMain::actionSelectDrill, this)), 150),
        Cell::rel(1)
      }), 30),
      Cell::abs(10),
      Cell::abs(Row::make({
        Cell::rel(1),
        Cell::abs(Button::make("Play", std::bind(&ScreenMain::actionPlay, this)), 150),
        Cell::rel(1)
      }), 30),
      Cell::rel(1),
    }), 400),
    Cell::rel(1),
  }));

  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(1),
      Cell::abs(Label::make("AngryAim"), 150),
      Cell::rel(Row::make({
        Cell::rel(1),
        Cell::abs(Button::make("Settings", std::bind(&ScreenMain::actionSettings, this)), 120),
      }), 1),
    }), 30),
    Cell::rel(),
  }));
}
// @formatter:on

void ScreenMain::actionSelectDrill() {
  if (drillListContainer->children.empty()) {
    drillListContainer->add(constructDrillList());
    drillListContainer->layout();
  }
}

void ScreenMain::actionPlay() {
  Application::app.updateScene(std::make_shared<SceneDrill>(drill));
}

void ScreenMain::actionSettings() {
  Application::app.setScreen(std::make_shared<ScreenSettings>(Application::app.getScreen()));
}

void ScreenMain::handle(UiEvent &event) {
  if (event.type == UiEventType::KEY && event.down) {
    if (event.button == GLFW_KEY_SPACE || event.button == GLFW_KEY_ENTER)
      Application::app.later(std::bind(&ScreenMain::actionPlay, this));
    if (event.button == GLFW_KEY_TAB)
      Application::app.later(std::bind(&ScreenMain::actionSelectDrill, this));
    if (event.button == GLFW_KEY_1)
      Application::app.updateScene(std::make_shared<SceneBackpack>());
  }

  Ui::handle(event);
}
