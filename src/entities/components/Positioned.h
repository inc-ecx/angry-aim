#ifndef POSITIONED_H
#define POSITIONED_H

#include "inc/inc_glm.h"

class Positioned {
public:
  virtual ~Positioned() = default;
  glm::vec3 pos;
};

#endif //POSITIONED_H
