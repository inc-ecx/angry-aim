#include "DebugInfo.h"

std::map<std::string, DebugLine> DebugInfo::_info;

const std::map<std::string, DebugLine> &DebugInfo::info = _info;

void DebugInfo::put(const std::string &name, const std::string &value) {
  _info[name] = DebugLine{
    msCurrent(),
    value
  };
}

void DebugInfo::update() {
  uint64_t msNow = msCurrent();

  for (auto it = _info.cbegin(); it != _info.cend();) {
    if ((msNow - it->second.msUpdated) > MAX_AGE_MS)
      _info.erase(it++);
    else
      ++it;
  }
}
