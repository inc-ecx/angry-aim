#ifndef UTIL_DRILL_H
#define UTIL_DRILL_H

#include "shared/HitController.h"

class UtilDrill {
public:
  static void showResults(const std::string& drill, const std::shared_ptr<HitController>& ctrlHit);
};

#endif //UTIL_DRILL_H
