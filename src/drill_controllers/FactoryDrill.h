#ifndef FACTORY_DRILLS_H
#define FACTORY_DRILLS_H
#include <functional>
#include <memory>

#include "drill/controller/DrillController.h"
#include "drill/model/Drill.h"

struct Entry {
  std::function<std::shared_ptr<DrillController>(std::string args)> creator;
};

class FactoryDrill {
  static std::map<std::string, Entry> entries;
public:
  static std::shared_ptr<DrillController> create(const Drill& drill);
};

#endif //FACTORY_DRILLS_H
