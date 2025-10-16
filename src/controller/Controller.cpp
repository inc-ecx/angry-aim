#include "Controller.h"

Controller::Controller() {
}

void Controller::add(const std::shared_ptr<Entity> &entity) {
  world.entities.insert(entity);
  controllerMap[entity] = ControllerEntry();
}

void Controller::control(const std::shared_ptr<Entity> &entity, const std::shared_ptr<EntityController> &controller) {
  controllerMap[entity].controllers.push_back(controller);
}

void Controller::remove(const std::shared_ptr<Entity> &entity) {
  world.entities.erase(entity);
  controllerMap.erase(entity);
}

void Controller::update(double dt) {
  for (auto [entity, entry] : controllerMap) {
    for (auto controller : entry.controllers) controller->update(dt);
  }
}

void Controller::handle(const UiEvent &event) {
  for (auto [entity, entry] : controllerMap) {
    for (auto controller : entry.controllers) controller->handle(event);
  }
}
