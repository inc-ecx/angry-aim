#ifndef APPLICATION_H
#define APPLICATION_H

#include <functional>

#include <limits>
#include <memory>
#include <mutex>

#include "inc/inc_glfw.h"

#include "render/RenderUi.h"
#include "render/RenderFont.h"
#include "render/RenderScene.h"
#include "render/RenderSceneDrill.h"

#include "scenes/Scene.h"

#include "ui/Ui.h"
#include "ui/UiEvent.h"

#include "DebugInfo.h"

#include "util/Observable.h"

#include "lib/miniaudio.h"
#include "sound/Sound.h"

struct Scissors {
  int x;
  int y;
  int w;
  int h;
};

class Application {
  // tasks
  std::mutex mtx;
  std::vector<std::function<void()> > laterVec;

  // loop configuration
  int targetFps = 432;
  int maxFpsQueue = 2;

  // window props
  int mouseX = std::numeric_limits<int>::min();
  int mouseY = std::numeric_limits<int>::min();
  int width = std::numeric_limits<int>::min();
  int height = std::numeric_limits<int>::min();

  // fps measure
  std::vector<std::chrono::time_point<std::chrono::steady_clock> > frameHistory;
  double frameHistoryDuration = 1.0;
  double currentFpsUpdate = 0.5;
  double currentFps = 0;

  // controllers
  std::shared_ptr<Ui> currentUi = nullptr;
  std::shared_ptr<Scene> currentScene = nullptr;

  void renderApp(double dt);

  void initApp();

  void onResize();

public:
  // resources
  GLFWwindow *window = nullptr;
  FT_Library freetype = nullptr;
  ma_engine miniaudio;
  std::vector<std::shared_ptr<Observation>> observations;

  static Application app;

  // renderers
  RenderUi renderUi;
  RenderFont renderFont;
  RenderScene renderScene;
  RenderSceneDrill renderSceneDrill;

  // scissors state
  std::vector<Scissors> scissors;

  void applyFs();

  void onClose();

  void runApp();

  void later(const std::function<void()> &task);

  void setScreen(const std::shared_ptr<Ui> &screen);

  void updateScene(const std::shared_ptr<Scene> &scene);

  void setClipboardText(const std::string &str);

  bool getClipboardText(std::string &str);

  void pushScissors(int x, int y, int w, int h);

  void popScissors();

  int getMouseX() { return mouseX; }
  int getMouseY() { return mouseY; }
  int getWidth() { return width; }
  int getHeight() { return height; }

  void onEvent(UiEvent event);

  std::shared_ptr<Sound> loadSound(const std::string &path);

  std::shared_ptr<Ui> getScreen() { return currentUi; }

  std::shared_ptr<Scene> getScene() { return currentScene; }
};

#endif //APPLICATION_H
