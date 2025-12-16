#ifndef DRILLS_H
#define DRILLS_H

#include <vector>

#include "Drill.h"

class ManagerDrill {
public:
  std::vector<Drill> drills = {
    Drill("angry_aim", "strafe", "Strafe", "Valorant based shooting.", "angry_aim", "strafe", json{}),
    Drill("angry_aim", "simple", "Simple", "Simple scene.", "angry_aim", "simple", json{}),
    Drill("angry_aim", "micro", "Micro", "Micro flick training.", "angry_aim", "micro", json{}),
    Drill("angry_aim", "nano", "Nano", "Nano flick training.", "angry_aim", "micro", json{{"area", 1.5}}),
    Drill("angry_aim", "ace", "Ace", "Flick to five targets.", "angry_aim", "ace", json{}),
    Drill("angry_aim", "flick", "Flick", "Practice single flicks.", "angry_aim", "flick", {}),
    Drill("angry_aim", "track_line", "Track Line", "Track straight-moving targets.", "angry_aim", "track_line", json{}),
    Drill("angry_aim", "track_run", "Track Run", "Track running targets.", "angry_aim", "track_line", json{{"speed", 5.4}}),
    Drill("angry_aim", "track_pattern", "Track Pattern", "Track predictable pattern.", "angry_aim", "track_pattern", {}),
    Drill("angry_aim", "demo", "Demo", "Demo drill.", "angry_aim", "demo", {}),
  };
  Drill getDefault() {
    return Drill("angry_aim", "strafe", "Strafe", "Valorant based shooting.", "angry_aim", "strafe", json{});
  }

  static ManagerDrill inst;
};


#endif //DRILLS_H
