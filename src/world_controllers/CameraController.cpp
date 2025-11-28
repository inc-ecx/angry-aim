#include "CameraController.h"

#include "Application.h"
#include "cam_consts.h"

CameraController::CameraController(const std::shared_ptr<Looking> &looking) :
  Toggelable(true),
  looking(looking) {}

CameraController::CameraController(
  const std::shared_ptr<Looking> &looking, const std::function<void()> &handleChange
) :
  Toggelable(true),
  looking(looking),
  handleChange(handleChange) {}

void CameraController::handle(const UiEvent &event) {
  if (!isEnabled()) return;

  if (event.type == UiEventType::MOUSE_MOVE) {
    double degYaw = -event.mdx * pxToDeg();
    double degPitch = -event.mdy * pxToDeg();

    looking->yaw = std::remainder(looking->yaw + degYaw, 360);
    looking->pitch = std::clamp(looking->pitch + degPitch, -90.0, 90.0);
    if (handleChange) handleChange();
  }
}
