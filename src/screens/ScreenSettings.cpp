#include "ScreenSettings.h"

#include "../Application.h"
#include "renderers/RenderCrosshair.h"
#include "state/State.h"
#include "ui/Color.h"
#include "ui/Field.h"
#include "ui/PanelY.h"
#include "ui/Range.h"
#include "ui/Slider.h"

class UiCrosshair : public Ui {
public:
  void render(double dt, const UiRenderParams &params) override {
    auto &app = Application::app;
    auto &render = app.renderUi;

    render.start();
    render.color(0x00000020);

    if (params.toBuffer)
      glBlendFunc(GL_ONE, GL_ZERO);
    render.rect(x, y, width, height);
    if (params.toBuffer)
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    render.stop();
    RenderCrosshair::render(x, y, width, height);
  }
};

// @formatter:off
std::shared_ptr<Ui> constructSettingsList() {
  std::vector<std::shared_ptr<ItemY> > items;

  auto &settings = State::state.settings;

  int labelWidth = 250;
  int labelGap = 10;
  int settingsGap = 48;
  int groupGap = 10;
  int scrollGap = 5;

  std::shared_ptr<Range> rngFov;
  std::shared_ptr<Range> rngHorWidth, rngVerGap, rngHorGap, rngVerLength, rngHorLength, rngVerWidth;

  std::shared_ptr<Color> colorCross;

  items.push_back(ItemY::make(Label::make("Graphics", 0xffffff60, LabelAlign::LEFT), 36));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Max FPS", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(Range::make(settings.maxFps, 30, 1000, 1, [&](auto v) {
      settings.maxFps = static_cast<int>(v+0.001);
    })),
    Cell::abs(scrollGap)
  }), 25));
  items.push_back(ItemY::make(groupGap));

  items.push_back(ItemY::make(Label::make("View", 0xffffff60, LabelAlign::LEFT), 36));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("FOV", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngFov = Range::make(settings.fov, 10, 170, 0.01, [&](auto v) {
      settings.fov = v;
      settings.triggerChangeView();
    })),
    Cell::abs(scrollGap)
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(labelWidth + labelGap),
    Cell::abs(Button::make("Valorant", [&settings, rngFov]() {
      settings.fov = 103;
      rngFov->set(settings.fov);
    }), 100),
    Cell::rel(),
    Cell::abs(scrollGap)
  }), 25));
  items.push_back(ItemY::make(settingsGap));

  items.push_back(ItemY::make(Label::make("Controls", 0xffffff60, LabelAlign::LEFT), 36));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Sensitivity", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(Range::make(settings.sensitivity, 0.010, 4, 0.001, [&](auto v) { settings.sensitivity = v; })),
    Cell::abs(scrollGap)
  }), 25));
  items.push_back(ItemY::make(settingsGap));

  items.push_back(ItemY::make(Label::make("Sound", 0xffffff60, LabelAlign::LEFT), 36));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Main Volume", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(Range::make(sqrt(settings.volume), 0, 1.3, 0.001, [&](auto v) {
      settings.volume = v * v;
      settings.onChangeVolume->trigger();
    })),
    Cell::abs(scrollGap)
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Hit Volume", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(Range::make(sqrt(settings.hitVolume), 0, 1.3, 0.001, [&](auto v) {
      settings.hitVolume = v * v;
    })),
    Cell::abs(scrollGap)
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Miss Volume", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(Range::make(sqrt(settings.missVolume), 0, 1.3, 0.001, [&](auto v) {
      settings.missVolume = v * v;
    })),
    Cell::abs(scrollGap)
  }), 25));
  items.push_back(ItemY::make(settingsGap));

  items.push_back(ItemY::make(Label::make("Crosshair", 0xffffff60, LabelAlign::LEFT), 36));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(std::make_shared<UiCrosshair>()),
    Cell::abs(scrollGap)
  }), 100));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Horizontal Line Width", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngHorWidth = Range::make(settings.crosshairXThickness, 0, 8, 1, [&](auto v){settings.crosshairXThickness = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Vertical Line Width", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngVerWidth = Range::make(settings.crosshairYThickness, 0, 8, 1, [&](auto v){settings.crosshairYThickness = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Horizontal Line Length", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngHorLength = Range::make(settings.crosshairXLength, 0, 24, 1, [&](auto v){settings.crosshairXLength = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Vertical Line Length", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngVerLength = Range::make(settings.crosshairYLength, 0, 24, 1, [&](auto v){settings.crosshairYLength = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Horizontal Gap", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngHorGap = Range::make(settings.crosshairXGap, 0, 16, 1, [&](auto v){settings.crosshairXGap = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Vertical Gap", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngVerGap = Range::make(settings.crosshairYGap, 0, 16, 1, [&](auto v){settings.crosshairYGap = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Color", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(colorCross = Color::make(settings.crosshairRgba, [&](auto v){settings.crosshairRgba = v;})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(labelWidth + labelGap),
    Cell::abs(Button::make("Default", [=, &settings]() {
      rngHorLength->set(settings.crosshairXLength = 4);
      rngHorWidth->set(settings.crosshairXThickness = 2);
      rngVerLength->set(settings.crosshairYLength = 4);
      rngVerWidth->set(settings.crosshairYThickness = 2);
      rngHorGap->set(settings.crosshairXGap = 4);
      rngVerGap->set(settings.crosshairYGap = 4);
      colorCross->setRgba(settings.crosshairRgba = 0x00ffffff);
    }), 100),
    Cell::rel(),
    Cell::abs(scrollGap)
  }), 25));
  items.push_back(ItemY::make(settingsGap));
  return PanelY::make(items);
}
// @formatter:on

// @formatter:off
ScreenSettings::ScreenSettings(const std::shared_ptr<Ui> &prev) : prev(prev)  {
  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(Row::make({
        Cell::abs(Button::make("Back", std::bind(&ScreenSettings::actionBack, this)), 120),
        Cell::rel(1),
      }), 1),
      // Cell::abs(Label::make("AngryAim"), 150),
      Cell::rel(1),
    }), 30),
    Cell::abs(40),
    Cell::rel(Row::make({
      Cell::rel(),
      Cell::abs(constructSettingsList(), 800),
      Cell::rel()
    })),
    Cell::abs(40),
  }));
}

void ScreenSettings::handle(UiEvent &event) {
  if (event.type == UiEventType::KEY && event.down && event.button == GLFW_KEY_ESCAPE) {
    std::shared_ptr<Ui> prev = this->prev;
    Application::app.later([prev] {
      Application::app.setScreen(prev);
    });
  }

  Ui::handle(event);
}

// @formatter:on

void ScreenSettings::actionBack() {
  Application::app.setScreen(prev);
}
