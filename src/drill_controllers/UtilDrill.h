#ifndef UTIL_DRILL_H
#define UTIL_DRILL_H

#include <random>

#include "shared/HitController.h"
#include "util/time_util.h"

class DrillRandom {
  std::default_random_engine rng;
  std::uniform_real_distribution<float> distFloat;
  std::uniform_real_distribution<> distDouble;

public:
  DrillRandom() {
    uint32_t microsCurrent = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::steady_clock::now().time_since_epoch()
    ).count());
    rng.seed(microsCurrent);
  }

  int nextBool() {
    std::uniform_int_distribution dist(0, 1);
    return dist(rng) == 0;
  }

  int nextInt(int max) {
    std::uniform_int_distribution dist(0, max + 1);
    return dist(rng);
  }

  int nextInt(int min, int max) {
    std::uniform_int_distribution dist(min, max + 1);
    return dist(rng);
  }

  float nextFloat() {
    return distFloat(rng);
  }

  float nextFloat(float min, float max) {
    return min + (max - min) * distFloat(rng);
  }

  double nextDouble() {
    return distDouble(rng);
  }

  double nextDouble(double min, double max) {
    return min + (max - min) * distDouble(rng);
  }
};

class UtilDrill {
public:
  static void showResults(const std::string &drill, const std::shared_ptr<HitController> &ctrlHit);
  static void showResultsHit(const std::string &drill, const std::shared_ptr<HitController> &ctrlHit, uint64_t durationMs);
};

#endif //UTIL_DRILL_H
