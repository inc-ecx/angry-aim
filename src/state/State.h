#ifndef STATE_H
#define STATE_H

#include "util/file_util.h"

#include "StateDrill.h"
#include "ManagerState.h"
#include "StateSettings.h"

class State {
  void load(const fs::path &file, const std::vector<ManagerState *> &manager);

  void save(const fs::path &file, const std::vector<ManagerState *> &manager);

public:
  StateDrill drill;
  StateSettings settings;

  void loadAll();

  void saveAll();

  static State state;
};

#endif //STATE_H
