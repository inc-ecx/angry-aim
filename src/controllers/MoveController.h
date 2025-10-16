#ifndef MOVE_CONTROLLER_H
#define MOVE_CONTROLLER_H

#include <memory>

#include "components/Toggelable.h"
#include "entities/components/Positioned.h"
#include "entities/components/Looking.h"

#include "controller/EntityController.h"

class MoveController : public EntityController, public Toggelable {
public:
  MoveController(const std::shared_ptr<Positioned> &positioned, const std::shared_ptr<Looking> &looking);

  std::shared_ptr<Positioned> positioned;
  std::shared_ptr<Looking> looking;

  void update(double dt) override;
};

#endif //MOVE_CONTROLLER_H
