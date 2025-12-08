#ifndef STATE_DRILL_H
#define STATE_DRILL_H
#include "ManagerState.h"
#include "drill/model/Drill.h"
#include "drill/model/ManagerDrill.h"

class StateDrill : public ManagerState {
public:
  Drill currentDrill = ManagerDrill::inst.getDefault();

  void load(json &j) override;

  void save(json &j) override;
};

#endif //STATE_DRILL_H
