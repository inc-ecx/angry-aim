#include "WorldController.h"

#include <stdexcept>

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
  controllerMap[entity].controllers.push_back(controller);
}

void WorldController::remove(const std::shared_ptr<Entity> &entity) {
  if (entity == nullptr) return;
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
