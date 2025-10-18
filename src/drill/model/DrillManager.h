#ifndef DRILLS_H
#define DRILLS_H

#include <vector>

#include "Drill.h"

class DrillManager {
public:
  std::vector<Drill> drills = {
    Drill("Simple", "Simple scene.", "simple"),
    Drill("Default", "Valorant based shooting.", "default"),
  };
  Drill def = Drill("Default", "Valorant based shooting.", "default");
  static DrillManager inst;
};


DrillManager DrillManager::inst;


#endif //DRILLS_H
