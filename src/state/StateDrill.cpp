#include "StateDrill.h"

void StateDrill::load(json &j) {
  if (j["currentDrill"].is_string()) {
    std::string link = j["currentDrill"];
    for (auto drill : ManagerDrill::inst.drills) {
      if (drill.link == link) {
        currentDrill = drill;
        break;
      }
    }
  }
}

void StateDrill::save(json &j) {
  j["currentDrill"] = currentDrill.link;
}
