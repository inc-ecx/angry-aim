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
#include "render/RenderPrimitive.h"
#include "render/Texture.h"
#include "renderers/PipelineUi.h"
#include "sound/Sound.h"

class Application {
  struct Scissors {
    int x;
    int y;
    int w;
    int h;
  };

  struct Textures {
    std::unique_ptr<Texture> logoLarge;
  };

  // tasks
  std::mutex mtx;
  std::vector<std::function<void()> > laterVec;

  // window props
  int mouseX = std::numeric_limits<int>::min();
  int mouseY = std::numeric_limits<int>::min();
  int width = std::numeric_limits<int>::min();
  int height = std::numeric_limits<int>::min();
  bool windowFocused = false;

  // fps measure
  std::vector<std::chrono::time_point<std::chrono::steady_clock> > frameHistory;
  double frameHistoryDuration = 1.0;
  double currentFpsUpdate = 0.5;
  double currentFps = 0;

  // controllers
  std::shared_ptr<Ui> currentUi = nullptr;
  std::shared_ptr<Scene> currentScene = nullptr;
  std::chrono::time_point<std::chrono::steady_clock> lastFrame;
  std::chrono::time_point<std::chrono::steady_clock> lastFpsUpdate;

  void tickApp();

  void renderApp(double dt);

  void initApp();

  void onResize();

public:
  static constexpr float MAX_FPS_LIMIT = 2000;

  static Application app;

  // resources
  GLFWwindow *window = nullptr;
  FT_Library freetype = nullptr;
  ma_engine miniaudio;
  std::vector<std::shared_ptr<Observation>> observations;
  std::vector<Scissors> scissors;
  Textures textures;

  // pipelines
  // note: pipelines are responsible for managing a chain of renderers and presenting the final image
  PipelineUi pipelineUi;

  // renders
  // note: renders are responsible for providing the functionalities for drawing a scene/screen.
  RenderUi renderUi;
  RenderFont renderFont;
  RenderFont renderFontStats;
  RenderScene renderScene;
  RenderSceneDrill renderSceneDrill;
  RenderPrimitive renderPrimitive;

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

  static GLFWimage loadIcon(const std::string &location);
};

#endif //APPLICATION_H
