#include "CameraController.h"

#include "Application.h"

CameraController::CameraController(const std::shared_ptr<Looking> &looking) : Toggelable(true), looking(looking) {
}

void CameraController::handle(const UiEvent &event) {
  if (!isEnabled()) return;

  if (event.type == UiEventType::MOUSE_MOVE) {
    double sensitivity = baseSensitivity;
    double degYaw = -event.mdx * sensitivity * 0.07;
    double degPitch = -event.mdy * sensitivity * 0.07;

    looking->yaw = std::remainder(looking->yaw + degYaw, 360);
    looking->pitch = std::clamp(looking->pitch + degPitch, -90.0, 90.0);
  }
}
