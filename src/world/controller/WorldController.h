#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <vector>

#include "Controller.h"
#include "world/model/World.h"

struct ControllerEntry {
  std::vector<std::shared_ptr<Controller> > controllers;
};

class WorldController {
  std::map<std::shared_ptr<Entity>, ControllerEntry> controllerMap;

public:
  World world;

  WorldController();

  void add(const std::shared_ptr<Entity> &entity);

  void control(const std::shared_ptr<Controller> &controller);

  void control(const std::shared_ptr<Entity> &entity, const std::shared_ptr<Controller> &controller);

  void remove(const std::shared_ptr<Entity> &entity);

  // events

  void update(double dt);

  void handle(const UiEvent & event);
};

#endif //CONTROLLER_H
