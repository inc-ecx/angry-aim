#include "UtilDrill.h"

#include "Application.h"
#include "screens/ScreenResult.h"

//
// Tracking results
//

// @formatter:off
void UtilDrill::showResults(const Drill& drill, const std::shared_ptr<TrackController>& ctrlTrack) {
  auto &app = Application::app;
  app.later([=] {
    // Log::info(std::format("{:#010x}", reinterpret_cast<intptr_t>(ctrlTrack.get())));
    Application::app.setScreen(std::make_shared<ScreenResult>(ScreenResultArgs{
      .drill = drill,
      .drillProps = {
        {"drill", DrillFormat::formatDrillId(drill)}
      },
      .mainStats = {
        {"tracked", std::format("{:0.1f}%", static_cast<double>(ctrlTrack->stats.hoverMs) / std::max(UINT64_C(1), ctrlTrack->stats.hoverMs + ctrlTrack->stats.missMs) * 100)},
        {"steadiness", std::format("{:0.1f}%", static_cast<double>(ctrlTrack->stats.reachedHoverMs) / std::max(UINT64_C(1), ctrlTrack->stats.reachedHoverMs + ctrlTrack->stats.reachedMissMs) * 100)},
        {"ttr", std::format("{}", static_cast<int>(ctrlTrack->stats.timeToReachSumMs / ctrlTrack->stats.reachedCount))}
      }
   }));
 });
}
// @formatter:on

//
// Clicking results
//

// @formatter:off
void UtilDrill::showResults(const Drill& drill, const std::shared_ptr<HitController>& ctrlHit) {
  auto &app = Application::app;
  app.later([=] {
    Application::app.setScreen(std::make_shared<ScreenResult>(ScreenResultArgs{
      .drill = drill,
      .drillProps = {
        {"drill", DrillFormat::formatDrillId(drill)}
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
void UtilDrill::showResultsWithTime(const Drill& drill, const std::shared_ptr<HitController>& ctrlHit, uint64_t durationMs) {
  int duration = static_cast<int>((durationMs + 999) / 1000);
  int m = duration / 60;
  int s = duration % 60;

  auto &app = Application::app;
  app.later([drill, ctrlHit, m, s] {
    Application::app.setScreen(std::make_shared<ScreenResult>(ScreenResultArgs{
      .drill = drill,
      .drillProps = {
        {"drill", DrillFormat::formatDrillId(drill)}
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
