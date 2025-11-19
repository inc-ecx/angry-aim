#include "TimeController.h"
#include "Application.h"

#include "util/time_util.h"

TimeController::TimeController(TimeControllerArgs &&args):
  args(args) {}

//
// Own Functions
//

void TimeController::start() {
  started = true;
  msStarted = msCurrent();
  args.handleStart();
}

void TimeController::stop() {
  over = true;
  args.handleEnd();
}

//
// World Controller
//

void TimeController::update(double dt) {
  if (!started) return;

  uint64_t runningMs = msCurrent() - msStarted;
  int timeLeft = static_cast<int>((args.durationSeconds * 1000 - static_cast<int>(runningMs)) / 1000.0f + 0.5f);
  if (timeLeft <= 0) {
    if (!over) stop();
    return;
  }

  int m = timeLeft / 60;
  int s = timeLeft % 60;
  args.handleTime(std::format("{:02d}:{:02d}", m, s));
}

void TimeController::handle(const UiEvent &event) {
  if (event.type == UiEventType::MOUSE_BUTTON && event.button == GLFW_MOUSE_BUTTON_LEFT && event.down) {
    if (!started) {
      Application::app.later([this]() {start();});
    }
  }
}
