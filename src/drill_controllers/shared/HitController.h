#ifndef HIT_CONTROLLER_H
#define HIT_CONTROLLER_H
#include <complex.h>
#include <functional>

#include "entities/HitTarget.h"
#include "entities/MainPlayer.h"
#include "world/controller/Controller.h"
#include "world/controller/WorldController.h"
#include "world_controllers/components/Toggelable.h"


struct HitControllerArgs {
  WorldController* world;
  MainPlayer* player;
  std::function<void()> listenerMiss;
  std::function<void(const std::shared_ptr<HitTarget> &target)> listenerHit;
};

struct HitControllerStats {
  int hit = 0;
  int miss = 0;
  uint64_t ttkSum = 0;
};

class HitController : public Controller, public Toggelable {
  HitControllerArgs args;

  uint64_t msLastSpawn = 0;

  void onMiss();

  void onHit(const std::shared_ptr<HitTarget> & target);

public:
  HitControllerStats stats;

  explicit HitController(HitControllerArgs&& args);

  //
  // Input message
  //

  void notifyCanKill();

  void notifyOver();

  //
  // Controller implementation
  //

  void update(double dt) override;

  void handle(const UiEvent &event) override;
};

#endif //HIT_CONTROLLER_H
