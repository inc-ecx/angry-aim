#include "State.h"
#include <fstream>
#include "util/file_util.h"

#include "Log.h"

static const fs::path dirBase = "state";
static const fs::path fileDrill = "drill.json";
static const fs::path fileSettings = "settings.json";

State State::state;

void State::load(const fs::path &file, const std::vector<ManagerState *> &manager) {
  std::ifstream is(file);

  if (!is.is_open()) {
    return;
  }

  json j;
  try {
    j = json::parse(is);
  } catch (...) {
    Log::error(std::format("Failed to parse file \"{}\"", fs::absolute(file).string()));
    return;
  }
  for (auto mgr : manager) mgr->load(j);
}

void State::save(const fs::path &file, const std::vector<ManagerState *> &manager) {
  std::ofstream fs(file);
  if (!fs.is_open()) {
    Log::error(std::format("Failed to open file \"{}\"", fs::absolute(file).string()));
    return;
  }
  json j = {};
  for (auto mgr : manager) mgr->save(j);
  fs << std::setw(4) << j << std::endl;
}

void State::loadAll() {
  load(dirBase / fileDrill, {&drill});
  load(dirBase / fileSettings, {&settings});
}

void State::saveAll() {
  fs::create_directories(dirBase);
  save(dirBase / fileDrill, {&drill});
  save(dirBase / fileSettings, {&settings});
}
