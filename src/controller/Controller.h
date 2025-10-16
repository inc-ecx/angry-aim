#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <vector>

#include "EntityController.h"
#include "world/World.h"

struct ControllerEntry {
  std::vector<std::shared_ptr<EntityController> > controllers;
};

class Controller {
  std::map<std::shared_ptr<Entity>, ControllerEntry> controllerMap;

public:
  World world;

  Controller();

  void add(const std::shared_ptr<Entity> &entity);

  void control(const std::shared_ptr<Entity> &entity, const std::shared_ptr<EntityController> &controller);

  void remove(const std::shared_ptr<Entity> &entity);

  // events

  void update(double dt);

  void handle(const UiEvent & event);
};

#endif //CONTROLLER_H
