#include "FactoryDrill.h"

#include "DrillDefault.h"
#include "DrillSimple.h"

std::map<std::string, Entry> FactoryDrill::entries{
  {"default", {.creator = []() { return std::make_shared<DrillDefault>(); }}},
  {"simple", {.creator = []() { return std::make_shared<DrillSimple>(); }}},
};

std::shared_ptr<DrillController> FactoryDrill::create(const Drill &drill) {
  std::string id = drill.link;
  auto it = entries.find(id);
  if (it == entries.end()) return nullptr;
  // throw std::runtime_error(
  //   std::format("could not find drill controller for drill {} with id {}", drill.name, id));

  auto &entry = it->second;
  return entry.creator();
}
