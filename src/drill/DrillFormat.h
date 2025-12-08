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
};

#endif //DRILL_FORMAT_H
