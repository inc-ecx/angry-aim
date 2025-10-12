#ifndef APPLICATION_H
#define APPLICATION_H

#include "ui/Ui.h"

#include <limits>
#include <memory>

#include "inc/inc_glfw.h"

#include "render/RenderUi.h"
#include "render/RenderFont.h"

class Ui;

class Application {
  FT_Library freetype = nullptr;

  GLFWwindow *window = nullptr;
  int targetFps = 120;
  int maxFpsQueue = 2;
  int mouseX = std::numeric_limits<int>::min();
  int mouseY = std::numeric_limits<int>::min();
  int width = std::numeric_limits<int>::min();
  int height = std::numeric_limits<int>::min();

  std::shared_ptr<Ui> currentUi = nullptr;

  void renderApp();

  void initApp();

  void onResize();

public:
  static Application app;

  RenderUi renderUi;
  RenderFont renderFont;

  void runApp();

  int getMouseX() { return mouseX; }
  int getMouseY() { return mouseY; }
  int getWidth() { return width; }
  int getHeight() { return height; }
};

#endif //APPLICATION_H
