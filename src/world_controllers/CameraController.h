#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <memory>

#include "components/Toggelable.h"
#include "entities/components/Looking.h"
#include "world/controller/Controller.h"

class CameraController : public Controller, public Toggelable {

public:
  std::shared_ptr<Looking> looking;

  explicit CameraController(const std::shared_ptr<Looking> &looking);

  void handle(const UiEvent &event) override;

};

#endif //CAMERA_CONTROLLER_H
