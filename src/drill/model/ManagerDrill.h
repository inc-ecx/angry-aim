#ifndef DRILLS_H
#define DRILLS_H

#include <vector>

#include "Drill.h"

class ManagerDrill {
public:
  std::vector<Drill> drills = {
    Drill("Strafe", "Valorant based shooting.", "strafe"),
    Drill("Simple", "Simple scene.", "simple"),
    Drill("Micro", "Micro flick training.", "micro"),
    Drill("Nano", "Nano flick training.", "micro:1.5"),
    Drill("Ace", "Flick to five targets.", "ace"),
    Drill("Flick", "Practice single flicks.", "flick"),
    Drill("Track Line", "Track straight-moving targets.", "track_line"),
  };
  Drill def = Drill("Default", "Valorant based shooting.", "default");

  static ManagerDrill inst;
};


#endif //DRILLS_H
