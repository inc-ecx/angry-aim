#include "FactoryDrill.h"

#include "DrillAce.h"
#include "DrillDemo.h"
#include "DrillStrafe.h"
#include "DrillMicro.h"
#include "DrillSimple.h"
#include "DrillFlick.h"
#include "DrillTrackLine.h"
#include "DrillTrackPattern.h"

std::map<std::string, Entry> FactoryDrill::entries{
  {"angry_aim:strafe", {.creator = [](auto r) { return std::make_shared<DrillStrafe>(r); }}},
  {"angry_aim:simple", {.creator = [](auto r) { return std::make_shared<DrillSimple>(r); }}},
  {"angry_aim:micro", {.creator = [](auto r) { return std::make_shared<DrillMicro>(r); }}},
  {"angry_aim:ace", {.creator = [](auto r) { return std::make_shared<DrillAce>(r); }}},
  {"angry_aim:flick", {.creator = [](auto r) { return std::make_shared<DrillFlick>(r); }}},
  {"angry_aim:track_line", {.creator = [](auto r) { return std::make_shared<DrillTrackLine>(r); }}},
  {"angry_aim:track_pattern", {.creator = [](auto r) { return std::make_shared<DrillTrackPattern>(r); }}},
  {"angry_aim:demo", {.creator = [](auto r) { return std::make_shared<DrillDemo>(r); }}},
};

std::shared_ptr<DrillController> FactoryDrill::create(const Drill &drill) {
  std::string controllerId = DrillFormat::formatControllerId(drill.controllerPackage, drill.controllerName);

  auto it = entries.find(controllerId);
  if (it == entries.end()) return nullptr;
  // throw std::runtime_error(
  //   std::format("could not find drill controller for drill {} with id {}", drill.name, id));

  auto &entry = it->second;
  return entry.creator(drill);
}
