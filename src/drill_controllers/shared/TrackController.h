#ifndef TRACK_CONTROLLER_H
#define TRACK_CONTROLLER_H

#include <functional>
#include "entities/HitTarget.h"
#include "entities/MainPlayer.h"
#include "world/controller/Controller.h"
#include "world/controller/WorldController.h"
#include "world_controllers/components/Toggelable.h"

struct TrackControllerArgs {
  WorldController *world;
  MainPlayer *player;
  std::function<void(const std::shared_ptr<HitTarget>& target)> handleKilled;
};

struct TrackControllerStats {
  uint64_t hoverMs;
  uint64_t missMs;
  uint64_t reachedHoverMs;
  uint64_t reachedMissMs;
  uint64_t timeToReachSumMs = 0;
  int reachedCount = 0;
};

class TrackController :
    public Controller,
    public Toggelable {
  TrackControllerArgs args;

public:
  TrackControllerStats stats = {};

  // state
  bool canKill = false;
  bool hasReachedTarget = false;
  uint64_t msCanReach = 0;
  uint64_t msMeasure = 0;

  explicit TrackController(TrackControllerArgs &&args);

  void onInitialReach();

  //
  // Controller implementation
  //

  void update(double dt) override;

  void handle(const UiEvent &event) override;

  //
  // Track Controller methods
  //

  void onDisable() override;

  void setCanKill(bool canKill);
};

#endif //TRACK_CONTROLLER_H
