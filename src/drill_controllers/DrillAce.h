#ifndef DRILL_ACE_H
#define DRILL_ACE_H

#include <complex.h>

#include "drill/controller/DrillController.h"
#include "shared/TimeController.h"
#include "world_controllers/CameraController.h"

class DrillAce : public DrillController, public Controller {
  // args
  DrillControllerSetupArgs args;

  // controllers
  std::shared_ptr<TimeController> ctrlTime;
  std::shared_ptr<CameraController> ctrlCam;

  // state

public:
  explicit DrillAce(const std::string& args);

  // Own events

  void handleTime(const std::string& time);

  void handleTimeStart();

  void handleTimeEnd();

  //
  // drill controller
  //

  void setup(const DrillControllerSetupArgs &args) override;

  void pause() override;

  void resume() override;

  //
  // world controller
  //

  void update(double dt) override;

  void handle(const UiEvent &event) override;

};

#endif //DRILL_ACE_H
