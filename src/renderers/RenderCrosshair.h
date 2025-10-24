#ifndef RENDER_CROSSHAIR_H
#define RENDER_CROSSHAIR_H
#include "Application.h"
#include "state/State.h"
#include "render/RenderUi.h"

class RenderCrosshair {
public:
  static void render(int x, int y, int w, int h) {
    auto &app = Application::app;
    auto &renderUi = app.renderUi;

    renderUi.start();

    renderUi.color(0x00ffffff);

    auto &settings = State::state.settings;
    int chWidth = settings.crosshairXLength * 2 + settings.crosshairXGap;
    int chHeight = settings.crosshairYLength * 2 + settings.crosshairYGap;
    int xLen = settings.crosshairXLength;
    int yLen = settings.crosshairYLength;
    // top left coords
    int tlx = x + static_cast<int>(round((w - chWidth) / 2.0));
    int tly = y + static_cast<int>(round((h - chHeight) / 2.0));

    // left
    renderUi.rect(
      tlx, static_cast<int>(tly + (chHeight - settings.crosshairYThickness) / 2.0), xLen, settings.crosshairYThickness
    );
    // right
    renderUi.rect(
      tlx + chWidth - xLen, static_cast<int>(tly + (chHeight - settings.crosshairYThickness) / 2.0), xLen,
      settings.crosshairYThickness
    );
    // top
    renderUi.rect(
      static_cast<int>(tlx + (chWidth - settings.crosshairXThickness) / 2.0), tly, settings.crosshairXThickness, yLen
    );
    // bottom
    renderUi.rect(
      static_cast<int>(tlx + (chWidth - settings.crosshairXThickness) / 2.0), tly + chHeight - yLen,
      settings.crosshairXThickness, yLen
    );

    renderUi.stop();
  }
};

#endif //RENDER_CROSSHAIR_H
