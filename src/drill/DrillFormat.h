#ifndef DRILL_FORMAT_H
#define DRILL_FORMAT_H

#include <format>
#include "drill/model/Drill.h"

class DrillFormat {
public:
  static std::string formatDrillId(const Drill& drill) {
    return std::format("{}/{}", drill.package, drill.name);
  }

  static std::string formatControllerId(std::string package, std::string name){
    return std::format("{}:{}", package, name);
  }

  static std::string formatSearchValue(const Drill& drill) {
    return std::format("{} {} {}/{} {}:{}", drill.name, drill.title, drill.package, drill.name, drill.controllerPackage, drill.controllerName);
  }
};

#endif //DRILL_FORMAT_H
