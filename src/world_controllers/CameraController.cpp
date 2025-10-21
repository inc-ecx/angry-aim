#include "CameraController.h"

#include "Application.h"
#include "cam_consts.h"

CameraController::CameraController(const std::shared_ptr<Looking> &looking) : Toggelable(true), looking(looking) {
}

void CameraController::handle(const UiEvent &event) {
  if (!isEnabled()) return;

  if (event.type == UiEventType::MOUSE_MOVE) {
    double sensitivity = camSens;
    double degYaw = -event.mdx * sensitivity * camPxToDeg;
    double degPitch = -event.mdy * sensitivity * camPxToDeg;

    looking->yaw = std::remainder(looking->yaw + degYaw, 360);
    looking->pitch = std::clamp(looking->pitch + degPitch, -90.0, 90.0);
  }
}
