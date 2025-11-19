#include "FactoryDrill.h"

#include "DrillAce.h"
#include "DrillDefault.h"
#include "DrillMicro.h"
#include "DrillSimple.h"

std::map<std::string, Entry> FactoryDrill::entries{
  {"default", {.creator = [](auto s) { return std::make_shared<DrillDefault>(); }}},
  {"simple", {.creator = [](auto s) { return std::make_shared<DrillSimple>(); }}},
  {"micro", {.creator = [](auto s) { return std::make_shared<DrillMicro>(s); }}},
  {"ace", {.creator = [](auto s) { return std::make_shared<DrillAce>(s); }}},
};

std::shared_ptr<DrillController> FactoryDrill::create(const Drill &drill) {
  std::string id;
  std::string args;

  // parse link
  {
    auto sep = drill.link.find(':');
    if (sep == std::string::npos) {
      id = drill.link;
      args = "";
    } else {
      id = drill.link.substr(0, sep);
      args = drill.link.substr(sep+1);
    }
  }

  auto it = entries.find(id);
  if (it == entries.end()) return nullptr;
  // throw std::runtime_error(
  //   std::format("could not find drill controller for drill {} with id {}", drill.name, id));

  auto &entry = it->second;
  return entry.creator(args);
}
