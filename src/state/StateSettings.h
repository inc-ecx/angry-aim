#ifndef STATE_SETTINGS_H
#define STATE_SETTINGS_H
#include "ManagerState.h"

class StateSettings : public ManagerState {
public:
  double fov = 103;

  void load(json &j) override;

  void save(json &j) override;
};

#endif //STATE_SETTINGS_H
