#ifndef APPLICATION_H
#define APPLICATION_H

#include <functional>

#include "ui/Ui.h"

#include <limits>
#include <memory>
#include <mutex>

#include "inc/inc_glfw.h"

#include "render/RenderUi.h"
#include "render/RenderFont.h"
#include "render/RenderScene.h"
#include "render/RenderSceneDefault.h"
#include "scene/Scene.h"
#include "ui/UiEvent.h"

class Ui;

class Application {
  std::mutex mtx;
  std::vector<std::function<void()> > laterVec;

  FT_Library freetype = nullptr;

  int targetFps = 432;
  int maxFpsQueue = 2;
  int mouseX = std::numeric_limits<int>::min();
  int mouseY = std::numeric_limits<int>::min();
  int width = std::numeric_limits<int>::min();
  int height = std::numeric_limits<int>::min();

  // fps measure
  std::vector<std::chrono::time_point<std::chrono::system_clock> > frameHistory;
  double frameHistoryDuration = 1.0;
  double currentFpsUpdate = 0.5;
  double currentFps = 0;

  std::shared_ptr<Ui> currentUi = nullptr;

  std::shared_ptr<Scene> currentScene = nullptr;

  void renderApp(double dt);

  void initApp();

  void onResize();

public:
  GLFWwindow *window = nullptr;

  static Application app;

  RenderUi renderUi;
  RenderFont renderFont;
  RenderScene renderScene;
  RenderSceneDefault renderSceneDef;

  void runApp();

  void later(const std::function<void()> &task);

  void setScreen(const std::shared_ptr<Ui> &screen);

  void setScene(const std::shared_ptr<Scene> &scene);

  int getMouseX() { return mouseX; }
  int getMouseY() { return mouseY; }
  int getWidth() { return width; }
  int getHeight() { return height; }

  void onEvent(UiEvent event);

  std::shared_ptr<Ui> getScreen() { return currentUi; }
};

#endif //APPLICATION_H
