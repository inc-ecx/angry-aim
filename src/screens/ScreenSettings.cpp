#include "ScreenSettings.h"

#include "../Application.h"
#include "renderers/RenderCrosshair.h"
#include "state/State.h"
#include "ui/Field.h"
#include "ui/PanelY.h"
#include "ui/Range.h"
#include "ui/Slider.h"

class UiCrosshair : public Ui {
public:
  void render(double dt) override {
    auto &app = Application::app;
    auto &render = app.renderUi;

    render.start();
    render.color(0x00000020);
    render.rect(x, y, width, height);
    render.stop();
    RenderCrosshair::render(x, y, width, height);
  }
};

// @formatter:off
std::shared_ptr<Ui> constructSettingsList() {
  std::vector<std::shared_ptr<ItemY> > items;

  auto &settings = State::state.settings;

  int labelWidth = 140;
  int labelGap = 10;
  int settingsGap = 20;
  int groupGap = 5;
  int scrollGap = 5;

  std::shared_ptr<Range> rngFov;
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("FOV", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngFov = Range::make(settings.fov, 10, 170, 0.01, [&](auto v){settings.fov = v;})),
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

  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Sensitivity", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(Range::make(settings.sensitivity, 0.010, 4, 0.001, [&](auto v){settings.sensitivity = v;})),
    Cell::abs(scrollGap)
  }), 25));

  items.push_back(ItemY::make(settingsGap));
    std::shared_ptr<Range> rngHorWidth, rngVerGap, rngHorGap, rngVerLength, rngHorLength, rngVerWidth;
    items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Crosshair", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(std::make_shared<UiCrosshair>()),
    Cell::abs(scrollGap)
  }), 100));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Hor Width", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngHorWidth = Range::make(settings.crosshairXThickness, 0, 8, 1, [&](auto v){settings.crosshairXThickness = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Ver Width", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngVerWidth = Range::make(settings.crosshairYThickness, 0, 8, 1, [&](auto v){settings.crosshairYThickness = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Hor Length", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngHorLength = Range::make(settings.crosshairXLength, 0, 24, 1, [&](auto v){settings.crosshairXLength = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Ver Length", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngVerLength = Range::make(settings.crosshairYLength, 0, 24, 1, [&](auto v){settings.crosshairYLength = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Hor Gap", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngHorGap = Range::make(settings.crosshairXGap, 0, 16, 1, [&](auto v){settings.crosshairXGap = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(Label::make("Ver Gap", LabelAlign::RIGHT), labelWidth),
    Cell::abs(labelGap),
    Cell::rel(rngVerGap = Range::make(settings.crosshairYGap, 0, 16, 1, [&](auto v){settings.crosshairYGap = static_cast<int>(v+0.001);})),
    Cell::abs(scrollGap),
  }), 25));
  items.push_back(ItemY::make(groupGap));
  items.push_back(ItemY::make(Row::make({
    Cell::abs(labelWidth + labelGap),
    Cell::abs(Button::make("Reset", [=, &settings]() {
      settings.crosshairXLength = 4;
      rngHorLength->set(4);
      settings.crosshairXThickness = 2;
      rngHorWidth->set(2);
      settings.crosshairYLength = 4;
      rngVerLength->set(4);
      settings.crosshairYThickness = 2;
      rngVerWidth->set(2);
      settings.crosshairXGap = 4;
      rngHorGap->set(4);
      settings.crosshairYGap = 4;
      rngVerGap->set(4);
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
      Cell::abs(Label::make("AngryAim"), 150),
      Cell::rel(1),
    }), 30),
    Cell::abs(40),
    Cell::rel(Row::make({
      Cell::rel(),
      Cell::abs(constructSettingsList(), 500),
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
