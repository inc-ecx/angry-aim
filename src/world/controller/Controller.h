#ifndef ENTITY_CONTROLLER_H
#define ENTITY_CONTROLLER_H

#include "ui/UiEvent.h"

class Controller {
public:
  virtual ~Controller() = default;

  virtual void update(double dt) {
  }

  virtual void handle(const UiEvent &event) {
  }
};


#endif //ENTITY_CONTROLLER_H
