#ifndef DRILLS_H
#define DRILLS_H

#include <vector>

#include "Drill.h"

class DrillManager {
public:
  std::vector<Drill> drills = {
    Drill("Strafe", "Valorant based shooting.", "default"),
    Drill("Simple", "Simple scene.", "simple"),
    Drill("Micro", "Micro flick training.", "micro"),
    Drill("Nano", "Nano flick training.", "micro:1.5"),
    Drill("Ace", "Flick to five targets.", "ace"),
  };
  Drill def = Drill("Default", "Valorant based shooting.", "default");

  static DrillManager inst;
};


#endif //DRILLS_H
