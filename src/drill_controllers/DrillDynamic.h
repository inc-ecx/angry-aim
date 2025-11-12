#ifndef DRILL_DYNAMIC_H
#define DRILL_DYNAMIC_H
#include "drill/controller/DrillController.h"

class DrillDynamic : public DrillController {
public:
  DrillDynamic() :
    DrillController("dynamic") {
  }
};

#endif //DRILL_DYNAMIC_H
