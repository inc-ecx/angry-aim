#include "Application.h"

#include <sstream>
#include <chrono>
#include <algorithm>

#include "Log.h"
#include "screens/ScreenMain.h"
#include "state/State.h"
#include "ui/UiEvent.h"

Application Application::app;

GLFWimage Application::loadIcon(const std::string &location) {
  GLFWimage res{};

  int nrChannels;
  unsigned char *data = stbi_load(location.c_str(), &res.width, &res.height, &nrChannels, 4);
  if (data == nullptr) {
    Log::error("Failed to load icon: " + location);
    return res;
  }

  res.pixels = data;
  return res;
}

void Application::initApp() {
  State::state.loadAll();

  if (!glfwInit()) {
    Log::fatal("Failed to initialize glfw.");
    glfwTerminate();
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  window = glfwCreateWindow(width = 960, height = 540, "AngryAim", nullptr, nullptr);
  if (!window) {
    Log::fatal("Failed to open GLFW window.");
    glfwTerminate();
    return;
  }

  GLFWimage images[4];
  images[0] = loadIcon("assets/ui/Logo Simple 16.png");
  images[1] = loadIcon("assets/ui/Logo Simple 32.png");
  images[2] = loadIcon("assets/ui/Logo Simple 48.png");
  images[3] = loadIcon("assets/ui/Logo Simple 256.png");
  glfwSetWindowIcon(window, 2, images);

  glfwSetCursorPosCallback(
    window, [](GLFWwindow *, double x, double y) {
      int dx = app.mouseX == std::numeric_limits<int>::min() ? 0 : (static_cast<int>(x) - app.mouseX);
      int dy = app.mouseY == std::numeric_limits<int>::min() ? 0 : (static_cast<int>(y) - app.mouseY);
      app.mouseX = static_cast<int>(x);
      app.mouseY = static_cast<int>(y);
      app.onEvent(UiEvent(app.mouseX, app.mouseY, dx, dy));
    }
  );
  double mx, my;
  glfwGetCursorPos(window, &mx, &my);
  mouseX = static_cast<int>(mx);
  mouseY = static_cast<int>(my);

  glfwSetWindowSizeCallback(
    window, [](GLFWwindow *, int w, int h) {
      app.width = w;
      app.height = h;
      app.onResize();
    }
  );

  glfwSetMouseButtonCallback(
    window, [](GLFWwindow *, int button, int action, int mods) {
      if (action == GLFW_PRESS || action == GLFW_RELEASE) {
        app.onEvent(UiEvent(UiEventType::MOUSE_BUTTON, action == GLFW_PRESS, button));
      }
    }
  );

  glfwSetKeyCallback(
    window, [](GLFWwindow *, int key, int scancode, int action, int mods) {
      if (action == GLFW_PRESS || action == GLFW_RELEASE || action == GLFW_REPEAT) {
        app.onEvent(UiEvent(UiEventType::KEY, action != GLFW_RELEASE, key));
      }
    }
  );

  glfwSetCharCallback(
    window, [](GLFWwindow *, uint32_t codepoint) {
      app.onEvent(UiEvent(codepoint));
    }
  );

  glfwSetScrollCallback(
    window, [](GLFWwindow *, double dx, double dy) {
      app.onEvent(UiEvent(dx, -dy));
    }
  );

  glfwSetWindowFocusCallback(
    window,
    [](GLFWwindow *, int focused) {
      app.windowFocused = focused ? true : false;
    }
  );

  glfwMakeContextCurrent(window);

  gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

  FT_Error ftErr = FT_Init_FreeType(&freetype);
  if (ftErr) {
    Log::fatal("Failed to initialize freetype.");
    return;
  }

  ma_result res = ma_engine_init(nullptr, &miniaudio);
  if (res != MA_SUCCESS) {
    Log::fatal("Failed to initialize miniaudio.");
    return;
  }

  ma_engine_set_volume(&miniaudio, static_cast<float>(State::state.settings.volume));
  observations.push_back(
    State::state.settings.onChangeVolume->listen(
      [this] {
        ma_engine_set_volume(&miniaudio, static_cast<float>(State::state.settings.volume));
      }
    )
  );
  observations.push_back(State::state.settings.onChangeFullscreen->listen(std::bind(&Application::applyFs, this)));

  stbi_set_flip_vertically_on_load(true);

  renderUi.init();
  renderScene.init();
  renderSceneDrill.init();
  renderFont.init(freetype, "assets/fonts/RobotoMono-Regular.ttf", 18);
  renderFontStats.init(freetype, "assets/fonts/RobotoMono-Regular.ttf", 40);
  renderPrimitive.init();

  pipelineUi.init();

  textures.logoLarge = Texture::load("assets/ui/Logo Large.png");

  applyFs();

  onResize(); // resize renderers

  app.setScreen(std::make_shared<ScreenMain>());

  glfwShowWindow(window);

  glfwFocusWindow(window);
}

void Application::applyFs() {
  GLFWmonitor *mon = glfwGetWindowMonitor(window);
  bool isFullscreen = mon != nullptr;
  if (State::state.settings.fullscreen != isFullscreen) {
    if (mon == nullptr) {
      GLFWmonitor *primary = glfwGetPrimaryMonitor();
      const GLFWvidmode *mode = glfwGetVideoMode(primary);
      glfwSetWindowMonitor(window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
      int width = 1280;
      int height = 720;
      int xPos = 100;
      int yPos = 100;
      glfwSetWindowMonitor(window, nullptr, xPos, yPos, width, height, GLFW_DONT_CARE);
    }
  }
}

void Application::onResize() {
  if (width == 0 || height == 0) return;

  glViewport(0, 0, width, height);

  renderPrimitive.resize(width, height);
  renderUi.resize(width, height);
  renderScene.resize(width, height);
  renderSceneDrill.resize(width, height);
  renderFont.resize(width, height);
  renderFontStats.resize(width, height);

  pipelineUi.resize(width, height);

  if (currentUi != nullptr) {
    currentUi->setBounds(0, 0, width, height);
    currentUi->layout();
  }

  if (currentScene != nullptr) {
    currentScene->resize(width, height);
  }
}

void Application::runApp() {
  initApp();

  double fpsQueue = 1;

  auto lastIteration = std::chrono::steady_clock::now();
  lastFrame = std::chrono::steady_clock::now();
  lastFpsUpdate = std::chrono::steady_clock::now();

  auto &settings = State::state.settings;

  while (!glfwWindowShouldClose(window)) {
    auto currentIteration = std::chrono::steady_clock::now();
    double loopDt = std::chrono::duration<double>(currentIteration - lastIteration).count();
    lastIteration = currentIteration;

    int targetFps = settings.maxFps;
    int intervalMs = 0;
    if (!windowFocused) {
      targetFps = settings.maxTabbedOutFps;
      intervalMs = 5;
    } else if (currentUi != nullptr) {
      targetFps = settings.maxUiFps;
    }
    double maxFpsQueue = settings.maxFrameQueue;
    if (targetFps >= 30 && targetFps < 1000) {
      fpsQueue = std::min(fpsQueue + loopDt * targetFps, maxFpsQueue);
      while (fpsQueue >= 1) {
        fpsQueue--;
        tickApp();
      }
    } else {
      fpsQueue = 1;
      tickApp();
    }

    // note: this may take way longer than the time in milliseconds.
    // hence intervalMs is never > 0 if in game.
    if (intervalMs > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
  }

  onClose();
}

void Application::tickApp() {
  auto currentFrame = std::chrono::steady_clock::now();

  // get dt for frame
  double frameDt = std::chrono::duration<double>(currentFrame - lastFrame).count();
  lastFrame = currentFrame;
  frameDt = std::min(frameDt, 0.1);

  // calc fps
  frameHistory.push_back(currentFrame);

  // remove old frames from the front
  while (!frameHistory.empty() && std::chrono::duration<double>(currentFrame - frameHistory.front()).count() >
         frameHistoryDuration) {
    frameHistory.erase(frameHistory.begin());
  }

  if (frameHistory.size() > 2) {
    if (std::chrono::duration<double>(currentFrame - lastFpsUpdate).count() > currentFpsUpdate) {
      currentFps = (frameHistory.size() - 1) / std::chrono::duration<double>(currentFrame - frameHistory.front()).
                   count();
      lastFpsUpdate = currentFrame;
    }
  }

  renderApp(frameDt);

  glfwSwapBuffers(window);

  glfwPollEvents();

  std::vector<std::function<void()> > tasks; {
    std::lock_guard lk(this->mtx);
    tasks = this->laterVec;
    this->laterVec.clear();
  }
  for (auto task: tasks) task();
}

void Application::onClose() {
  // note: no cleaning here, because the app is assumed to be cleaned up by the os.
  State::state.saveAll();
}

void Application::later(const std::function<void()> &task) {
  std::lock_guard lk(this->mtx);
  laterVec.push_back(task);
}

void Application::onEvent(UiEvent event) {
  if (currentScene != nullptr) currentScene->handle(event);
  if (currentUi != nullptr) currentUi->handle(event);

  if (event.type == UiEventType::KEY && event.down && event.button == GLFW_KEY_F11) {
    State::state.settings.fullscreen = !State::state.settings.fullscreen;
    State::state.settings.onChangeFullscreen->trigger();
  }
}

void Application::setScreen(const std::shared_ptr<Ui> &screen) {
  currentUi = screen;
  pipelineUi.setMain(currentUi);

  if (screen != nullptr) {
    currentUi->setBounds(0, 0, width, height);
    currentUi->layout();
  }
}

void Application::updateScene(const std::shared_ptr<Scene> &scene) {
  if (currentScene != nullptr) {
    currentScene->close();
  }

  currentScene = scene;
  if (currentScene != nullptr) {
    setScreen(nullptr);
    currentScene->open();
    currentScene->resize(width, height);
  } else {
    setScreen(std::make_shared<ScreenMain>());
  }
}

void Application::setClipboardText(const std::string &str) {
  glfwSetClipboardString(window, str.c_str());
}

bool Application::getClipboardText(std::string &str) {
  const char *text = glfwGetClipboardString(window);
  if (text == nullptr) return false;
  str = text;
  return true;
}

void Application::renderApp(double dt) {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (currentScene != nullptr) {
    currentScene->render(dt);
  }

  // renders currentUi
  pipelineUi.render(dt);

  DebugInfo::update();
  std::stringstream ss;
  ss << static_cast<int>(round(currentFps));
  DebugInfo::put("FPS", ss.str());

  renderFont.start();

  int infoCount = static_cast<int>(DebugInfo::info.size());
  int gap = 2;
  int mb = 5;
  int ml = 4;
  int y = height - renderFont.height() * infoCount - gap * (infoCount - 1) - mb;
  int x = ml;

  for (auto info: DebugInfo::info) {
    int nx = x;
    renderFont.renderText(info.first, static_cast<float>(nx), static_cast<float>(y), 0xffffff80);

    int vx = renderFont.width(info.first) + static_cast<int>(renderFont.height() * 0.7f);
    renderFont.renderText(info.second.value, static_cast<float>(vx), static_cast<float>(y), 0xffffff80);

    y += renderFont.height() + gap;
  }

  renderFont.stop();
}

void Application::pushScissors(int x, int y, int w, int h) {
  // we use 0,0 as top left around here, but scissors uses 0,0 as bottom left
  y = height - y - h;

  // limit to scissors object before
  if (!scissors.empty()) {
    auto &back = scissors.back();
    int x1 = std::max(x, back.x);
    int y1 = std::max(y, back.y);
    int x2 = std::min(x + width, back.x + back.w);
    int y2 = std::min(y + height, back.y + back.h);
    x = x1;
    y = y1;
    w = x2 - x1;
    h = y2 - y1;
  }

  glScissor(x, y, w, h);
  if (scissors.empty())
    glEnable(GL_SCISSOR_TEST);
  scissors.push_back({x, y, w, h});

  if (scissors.size() > 10'000) throw std::runtime_error("scissor stack overflow");
}

void Application::popScissors() {
  scissors.pop_back();
  if (scissors.empty()) {
    glDisable(GL_SCISSOR_TEST);
  } else {
    auto &last = scissors.back();
    glScissor(last.x, last.y, last.w, last.h);
  }
}

std::shared_ptr<Sound> Application::loadSound(const std::string &path) {
  auto sound = std::make_shared<Sound>();
  auto result = ma_sound_init_from_file(&miniaudio, path.c_str(), 0, nullptr, nullptr, &sound->_ma_sound);
  if (result != MA_SUCCESS) {
    throw std::runtime_error(std::format("Failed to load sound {}", fs::absolute(path).string()));
  }
  return sound;
}
