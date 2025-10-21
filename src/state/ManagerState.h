#ifndef MANAGER_STATE_H
#define MANAGER_STATE_H

#include "inc/inc_json.h"

class ManagerState {
public:
  virtual ~ManagerState() = default;
  virtual void load(json &j) {}
  virtual void save(json &j) {}
};

#endif //MANAGER_STATE_H
