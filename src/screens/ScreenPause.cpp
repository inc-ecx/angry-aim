#include "ScreenPause.h"

#include "Application.h"
#include "Log.h"
#include "ScreenMain.h"
#include "ScreenSettings.h"

// TODO: usages of std::bind might very well be invalid here, because this could be destroyed.

ScreenPause::ScreenPause(std::weak_ptr<SceneDrill>&& scene) {
  this->_scene = std::move(scene);

  add(Row::make({
     Cell::rel(1),
     Cell::abs(Column::make({
       Cell::rel(1),
       Cell::abs(Row::make({
         Cell::rel(1),
         Cell::abs(Button::make("Resume", std::bind(&ScreenPause::actionResume, this)), 150),
         Cell::rel(1)
       }), 30),
       Cell::abs(10),
       Cell::abs(Row::make({
         Cell::rel(1),
         Cell::abs(Button::make("Restart", std::bind(&ScreenPause::actionRestart, this)), 150),
         Cell::rel(1)
       }), 30),
       Cell::abs(10),
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

void ScreenPause::handle(UiEvent &event) {
  auto &app = Application::app;

  if (event.type == UiEventType::KEY && event.down) {
    if (event.button == GLFW_KEY_ESCAPE) {
      app.later(std::bind(&ScreenPause::actionResume, this));
    } else if (event.button == GLFW_KEY_Q) {
      app.later(std::bind(&ScreenPause::actionQuit, this));
    } else if (event.button == GLFW_KEY_R) {
      app.later(std::bind(&ScreenPause::actionRestart, this));
    } else if (event.button == GLFW_KEY_S) {
      app.later(std::bind(&ScreenPause::actionSettings, this));
    }
  }

  Ui::handle(event);
}

//
// handlers
//

void ScreenPause::actionResume() {
  auto &app = Application::app;
  auto scene = _scene.lock();
  if (!scene) {
    Log::warn("ScreenPause::actionResume is miss-timed");
    return;
  }
  scene->resume();
  app.setScreen(nullptr); // NOTE: PLACEMENT HERE IMPORTANT TO NOT DESTROY THIS
}

void ScreenPause::actionQuit() {
  auto &app = Application::app;
  app.updateScene(nullptr);
  app.setScreen(std::make_shared<ScreenMain>()); // NOTE: PLACEMENT HERE IMPORTANT TO NOT DESTROY THIS
}

void ScreenPause::actionRestart() {
  auto &app = Application::app;
  Application::app.getScene()->replay();
  app.setScreen(nullptr); // NOTE: PLACEMENT HERE IMPORTANT TO NOT DESTROY THIS
}

void ScreenPause::actionSettings() {
  auto &app = Application::app;
  auto screen = dynamic_pointer_cast<ScreenPause>(app.getScreen());
  if (!screen) {
    Log::warn("ScreenPause::actionSettings is miss-timed");
    return;
  }
  app.setScreen(std::make_shared<ScreenSettings>(screen)); // NOTE: PLACEMENT HERE IMPORTANT TO NOT DESTROY THIS
}
