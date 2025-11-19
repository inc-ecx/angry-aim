#include "DrillAce.h"

#include "world_controllers/CameraController.h"

DrillAce::DrillAce(const std::string &args) :
  DrillController("ace") {
}

//
// own events
//

void DrillAce::handleTime(const std::string &time) {
  args.screen->lblMainStat->setText(time);
}

void DrillAce::handleTimeStart() {
}

void DrillAce::handleTimeEnd() {
  args.screen->lblMainStat->setText("");
}

//
// drill controller
//

void DrillAce::setup(const DrillControllerSetupArgs &args) {
  this->args = args;
  args.world->control(ctrlTime = std::make_shared<TimeController>(TimeControllerArgs{
    .durationSeconds = 60,
    .handleTime = std::bind(&DrillAce::handleTime, this, std::placeholders::_1),
    .handleStart = std::bind(&DrillAce::handleTimeStart, this),
    .handleEnd = std::bind(&DrillAce::handleTimeEnd, this)
  }));
  args.screen->lblMainStat->setText("Click to start");

  args.world->control(args.player, ctrlCam = std::make_shared<CameraController>(args.player));
}

void DrillAce::pause() {
  ctrlCam->setEnabled(false);
}

void DrillAce::resume() {
  ctrlCam->setEnabled(true);
}

//
// world controller
//

void DrillAce::update(double dt) {}

void DrillAce::handle(const UiEvent &event) {}
