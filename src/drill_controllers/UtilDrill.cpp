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