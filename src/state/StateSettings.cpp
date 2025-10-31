#include "StateSettings.h"

void StateSettings::load(json &j) {
  if (j["fullscreen"].is_boolean()) fullscreen = j["fullscreen"];

  if (j["fov"].is_number()) fov = j["fov"];
  if (j["sensitivity"].is_number()) sensitivity = j["sensitivity"];

  if (j["volume"].is_number()) volume = j["volume"];
  if (j["hitVolume"].is_number()) hitVolume = j["hitVolume"];
  if (j["missVolume"].is_number()) missVolume = j["missVolume"];

  if (j["crosshairXLength"].is_number()) crosshairXLength = j["crosshairXLength"];
  if (j["crosshairXThickness"].is_number()) crosshairXThickness = j["crosshairXThickness"];
  if (j["crosshairYLength"].is_number()) crosshairYLength = j["crosshairYLength"];
  if (j["crosshairYThickness"].is_number()) crosshairYThickness = j["crosshairYThickness"];
  if (j["crosshairXGap"].is_number()) crosshairXGap = j["crosshairXGap"];
  if (j["crosshairYGap"].is_number()) crosshairYGap = j["crosshairYGap"];
}

void StateSettings::save(json &j) {
  j["fullscreen"] = fullscreen;

  j["fov"] = fov;
  j["sensitivity"] = sensitivity;

  j["volume"] = volume;
  j["hitVolume"] = hitVolume;
  j["missVolume"] = missVolume;

  j["crosshairXLength"] = crosshairXLength;
  j["crosshairXThickness"] = crosshairXThickness;
  j["crosshairYLength"] = crosshairYLength;
  j["crosshairYThickness"] = crosshairYThickness;
  j["crosshairXGap"] = crosshairXGap;
  j["crosshairYGap"] = crosshairYGap;
}

void StateSettings::triggerChangeView() {
  onChangeView->trigger();
}

std::shared_ptr<Observation> StateSettings::listenChangeView(std::function<void()> listener) {
  return onChangeView->listen(std::move(listener));
}