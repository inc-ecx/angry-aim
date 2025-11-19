#ifndef TIME_CONTROLLER_H
#define TIME_CONTROLLER_H

#include "drill/controller/DrillController.h"
#include "world/controller/Controller.h"

struct TimeControllerArgs {
  int durationSeconds;
  std::function<void(const std::string &arg)> handleTime;
  std::function<void()> handleStart;
  std::function<void()> handleEnd;
};

class TimeController : public Controller {
  // args
  TimeControllerArgs args;

  // state
  bool started = false;
  bool over = false;
  uint64_t msStarted = 0;

public:
  explicit TimeController(TimeControllerArgs &&args);

  //
  // Own Functions
  //

private:

  void start();

  void stop();

public:

  bool isStarted() { return started; }

  bool isOver() { return over; }

  //
  // World Controller
  //

  void update(double dt) override;

  void handle(const UiEvent &event) override;
};

#endif //TIME_CONTROLLER_H
