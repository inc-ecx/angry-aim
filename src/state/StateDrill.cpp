#include "StateDrill.h"

void StateDrill::load(json &j) {
  if (j["currentDrillName"].is_string() && j["currentDrillPackage"].is_string()) {
    std::string package = j["currentDrillPackage"];
    std::string name = j["currentDrillName"];
    for (auto drill : ManagerDrill::inst.drills) {
      if (drill.package == package && drill.name == name) {
        currentDrill = drill;
        break;
      }
    }
  }
}

void StateDrill::save(json &j) {
  j["currentDrillPackage"] = currentDrill.package;
  j["currentDrillName"] = currentDrill.name;
}
