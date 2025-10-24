#ifndef STATE_SETTINGS_H
#define STATE_SETTINGS_H
#include "ManagerState.h"

class StateSettings : public ManagerState {
public:
  double fov = 103;
  double sensitivity = 0.3;

  int crosshairXLength = 4;
  int crosshairXThickness = 2;
  int crosshairYLength = 4;
  int crosshairYThickness = 2;
  int crosshairXGap = 4;
  int crosshairYGap = 4;

  void load(json &j) override;

  void save(json &j) override;
};

#endif //STATE_SETTINGS_H
