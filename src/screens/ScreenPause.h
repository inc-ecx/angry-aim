#ifndef SCREEN_PAUSE_H
#define SCREEN_PAUSE_H

#include "scenes/SceneDrill.h"

#include "ui/components.h"

class ScreenPause : public Ui, public std::enable_shared_from_this<ScreenPause> {
  std::weak_ptr<SceneDrill> _scene;

public:
  explicit ScreenPause(std::weak_ptr<SceneDrill>&& scene);

  void actionResume();
  void actionQuit();
  void actionSettings();
};

#endif //SCREEN_PAUSE_H
