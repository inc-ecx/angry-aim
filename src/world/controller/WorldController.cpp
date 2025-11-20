#include "WorldController.h"

#include <stdexcept>

#include "Log.h"

WorldController::WorldController() : controllerMap{
  {
    nullptr, ControllerEntry()
  }
} {
}

void WorldController::add(const std::shared_ptr<Entity> &entity) {
  world.entities.insert(entity);
  controllerMap[entity] = ControllerEntry();
}

// adds general controller
void WorldController::control(const std::shared_ptr<Controller> &controller) {
  controllerMap[nullptr].controllers.push_back(controller);
}

// adds controller, attached to entity
void WorldController::control(const std::shared_ptr<Entity> &entity, const std::shared_ptr<Controller> &controller) {
  if (!controllerMap.contains(entity)) {
    Log::error("Tried to control non-attached entity.");
    return;
  }
  controllerMap[entity].controllers.push_back(controller);
}

void WorldController::remove(const std::shared_ptr<Entity> &entity) {
  if (entity == nullptr) {
    Log::error("Tried to remove nullptr entity.");
    return;
  }
  world.entities.erase(entity);
  controllerMap.erase(entity);
}

void WorldController::update(double dt) {
  for (auto [entity, entry]: controllerMap) {
    for (auto controller: entry.controllers) controller->update(dt);
  }
}

void WorldController::handle(const UiEvent &event) {
  for (auto [entity, entry]: controllerMap) {
    for (auto controller: entry.controllers) controller->handle(event);
  }
}
