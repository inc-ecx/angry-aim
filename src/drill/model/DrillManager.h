#ifndef DRILLS_H
#define DRILLS_H

#include <vector>

#include "Drill.h"

class DrillManager {
public:
  std::vector<Drill> drills = {
    Drill("Default", "Valorant based shooting.", "default"),
    Drill("Simple", "Simple scene.", "simple"),
    Drill("Micro", "Micro flick training.", "micro"),
    Drill("Nano", "Nano flick training.", "micro:1.5"),
  };
  Drill def = Drill("Default", "Valorant based shooting.", "default");

  static DrillManager inst;
};


#endif //DRILLS_H
