
#ifndef CAM_CONSTS_H
#define CAM_CONSTS_H
#include "state/State.h"

// Same as in Valorant
static constexpr double camPxToDeg = 0.07;

constexpr double pxToDeg() {
  auto &settings = State::state.settings;
  return camPxToDeg * settings.sensitivity;
}

#endif //CAM_CONSTS_H
