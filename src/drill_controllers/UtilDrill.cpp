#include "UtilDrill.h"

#include "Application.h"
#include "screens/ScreenResult.h"
#include "shared/HitController.h"

// @formatter:off
void UtilDrill::showResults(const std::string& drill, const std::shared_ptr<HitController>& ctrlHit) {
  auto &app = Application::app;
  app.later([=] {
   Application::app.setScreen(std::make_shared<ScreenResult>(ScreenResultArgs{
     .drillProps = {
       {"drill", drill}
     },
     .mainStats = {
       {"ttk", std::format("{}ms", ctrlHit->stats.ttkSum / std::max(1, ctrlHit->stats.hit))},
       {"acc", std::format("{:0.1f}%", static_cast<float>(ctrlHit->stats.hit) / std::max(1, ctrlHit->stats.hit + ctrlHit->stats.miss) * 100.0f)},
       {"hit", std::format("{}", ctrlHit->stats.hit)},
     }
   }));
 });
}
// @formatter:on

// @formatter:off
void UtilDrill::showResultsHit(const std::string& drill, const std::shared_ptr<HitController>& ctrlHit, uint64_t durationMs) {
  int duration = static_cast<int>((durationMs + 999) / 1000);
  int m = duration / 60;
  int s = duration % 60;

  auto &app = Application::app;
  app.later([=] {
   Application::app.setScreen(std::make_shared<ScreenResult>(ScreenResultArgs{
     .drillProps = {
       {"drill", drill}
     },
     .mainStats = {
       {"ttk", std::format("{}ms", ctrlHit->stats.ttkSum / std::max(1, ctrlHit->stats.hit))},
       {"acc", std::format("{:0.1f}%", static_cast<float>(ctrlHit->stats.hit) / std::max(1, ctrlHit->stats.hit + ctrlHit->stats.miss) * 100.0f)},
       {"time", std::format("{:02d}:{:02d}", m, s)},
     }
   }));
 });
}
// @formatter:on