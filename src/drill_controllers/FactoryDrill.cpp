#include "FactoryDrill.h"

#include "DrillAce.h"
#include "DrillStrafe.h"
#include "DrillMicro.h"
#include "DrillSimple.h"
#include "DrillFlick.h"
#include "DrillTrackLine.h"
#include "DrillTrackPattern.h"

std::map<std::string, Entry> FactoryDrill::entries{
  {"angry_aim:strafe", {.creator = [](auto s) { return std::make_shared<DrillStrafe>(); }}},
  {"angry_aim:simple", {.creator = [](auto s) { return std::make_shared<DrillSimple>(); }}},
  {"angry_aim:micro", {.creator = [](auto s) { return std::make_shared<DrillMicro>(s); }}},
  {"angry_aim:ace", {.creator = [](auto s) { return std::make_shared<DrillAce>(); }}},
  {"angry_aim:flick", {.creator = [](auto s) { return std::make_shared<DrillFlick>(); }}},
  {"angry_aim:track_line", {.creator = [](auto s) { return std::make_shared<DrillTrackLine>(s); }}},
  {"angry_aim:track_pattern", {.creator = [](auto s) { return std::make_shared<DrillTrackPattern>(s); }}},
};

std::shared_ptr<DrillController> FactoryDrill::create(const Drill &drill) {
  std::string controllerId = std::format("{}:{}", drill.controllerPackage, drill.controllerName);
  json args = drill.controllerConfig;

  auto it = entries.find(controllerId);
  if (it == entries.end()) return nullptr;
  // throw std::runtime_error(
  //   std::format("could not find drill controller for drill {} with id {}", drill.name, id));

  auto &entry = it->second;
  return entry.creator(args);
}
