#include "ScreenPause.h"

#include "Application.h"
#include "ScreenSettings.h"

ScreenPause::ScreenPause(std::weak_ptr<SceneDrill>&& scene) {
  this->_scene = std::move(scene);

  add(Row::make({
     Cell::rel(1),
     Cell::abs(Column::make({
       Cell::rel(1),
       Cell::abs(10),
       Cell::abs(Row::make({
         Cell::rel(1),
         Cell::abs(Button::make("Resume", std::bind(&ScreenPause::actionResume, this)), 150),
         Cell::rel(1)
       }), 30),
       Cell::abs(Row::make({
         Cell::rel(1),
         Cell::abs(Button::make("Quit", std::bind(&ScreenPause::actionQuit, this)), 150),
         Cell::rel(1)
       }), 30),
       Cell::rel(1),
     }), 400),
     Cell::rel(1),
   }));

  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(1),
      Cell::rel(Row::make({
        Cell::rel(1),
        Cell::abs(Button::make("Settings", std::bind(&ScreenPause::actionSettings, this)), 120),
      }), 1),
    }), 30),
    Cell::rel(),
  }));
}

void ScreenPause::actionResume() {
  auto scene = _scene.lock();
  if (!scene) return;
  scene->resume();
}

void ScreenPause::actionQuit() {
  auto &app = Application::app;
  app.updateScene(nullptr);
}

void ScreenPause::actionSettings() {
  auto &app = Application::app;
  auto screen = dynamic_pointer_cast<ScreenPause>(app.getScreen());
  if (!screen) return;
  app.setScreen(std::make_shared<ScreenSettings>(screen));
}
