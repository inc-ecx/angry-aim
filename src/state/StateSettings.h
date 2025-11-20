#ifndef STATE_SETTINGS_H
#define STATE_SETTINGS_H

#include "ManagerState.h"
#include "util/Observable.h"

class StateSettings : public ManagerState {
  std::shared_ptr<Observable> onChangeView = Observable::make();

public:
  bool fullscreen = false;
  std::shared_ptr<Observable> onChangeFullscreen = Observable::make();
  int maxFps = 864;
  int maxUiFps = 144;
  int maxTabbedOutFps = 30;
  double maxFrameQueue = 2;

  double fov = 103;
  double sensitivity = 0.3;

  double volume = 0.2;
  std::shared_ptr<Observable> onChangeVolume = Observable::make();

  double hitVolume = 0.5;
  double missVolume = 1.0;

  int crosshairXLength = 4;
  int crosshairXThickness = 2;
  int crosshairYLength = 4;
  int crosshairYThickness = 2;
  int crosshairXGap = 4;
  int crosshairYGap = 4;
  int crosshairRgba = 0x00ffffff;

  void load(json &j) override;

  void save(json &j) override;

  std::shared_ptr<Observation> listenChangeView(std::function<void()> listener);

  void triggerChangeView();
};

#endif //STATE_SETTINGS_H
