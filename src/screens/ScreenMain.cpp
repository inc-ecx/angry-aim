#include "ScreenMain.h"

#include "ScreenScene.h"
#include "ScreenSettings.h"

#include "../Application.h"
#include "../scene/SceneBackpack.h"
#include "../scene/default/SceneDefault.h"

// @formatter:off
ScreenMain::ScreenMain() {
  add(Row::make({
    Cell::rel(1),
    Cell::abs(Column::make({
      Cell::rel(1),
      Cell::abs(Button::make("[Default]", std::bind(&ScreenMain::actionScene, this)), 30),
      Cell::abs(10),
      Cell::abs(Button::make("Play", std::bind(&ScreenMain::actionPlay, this)), 30),
      Cell::rel(1),
    }), 200),
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

void ScreenMain::actionScene() {
}

void ScreenMain::actionPlay() {
  Application::app.setScreen(std::make_shared<ScreenScene>());
  Application::app.setScene(std::make_shared<SceneDefault>());
}

void ScreenMain::actionSettings() {
  Application::app.setScreen(std::make_shared<ScreenSettings>(Application::app.getScreen()));
}
