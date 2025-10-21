#include "StateSettings.h"

void StateSettings::load(json &j) {
  if (j["fov"].is_number()) fov = j["fov"];
}

void StateSettings::save(json &j) {
  j["fov"] = fov;
}
