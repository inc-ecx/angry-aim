#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H

#include "util/time_util.h"
#include <string>
#include <map>

struct DebugLine {
  uint64_t msUpdated;
  std::string value;
};

class DebugInfo {
  static std::map<std::string, DebugLine> _info;

public:
  static constexpr int MAX_AGE_MS = 2000;
  static const std::map<std::string, DebugLine> &info;

  static void put(const std::string &name, const std::string &value);

  static void update();
};

#endif //DEBUG_INFO_H
