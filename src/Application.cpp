#include "Application.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "screens/ScreenMain.h"
#include "ui/UiEvent.h"

Application Application::app;

void Application::initApp() {
  if (!glfwInit())
    return;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(width = 640, height = 480, "AngryAim", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return;
  }

  glfwSetCursorPosCallback(window, [](GLFWwindow *, double x, double y) {
    int dx = app.mouseX == std::numeric_limits<int>::min() ? 0 : (static_cast<int>(x) - app.mouseX);
    int dy = app.mouseY == std::numeric_limits<int>::min() ? 0 : (static_cast<int>(y) - app.mouseY);
    app.mouseX = static_cast<int>(x);
    app.mouseY = static_cast<int>(y);
    app.onEvent(UiEvent(app.mouseX, app.mouseY, dx, dy));
  });
  double mx, my;
  glfwGetCursorPos(window, &mx, &my);
  mouseX = static_cast<int>(mx);
  mouseY = static_cast<int>(my);

  glfwSetWindowSizeCallback(window, [](GLFWwindow *, int w, int h) {
    app.width = w;
    app.height = h;
    app.onResize();
  });

  glfwSetMouseButtonCallback(window, [](GLFWwindow *, int button, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
      app.onEvent(UiEvent(UiEventType::MOUSE_BUTTON, action == GLFW_PRESS, button));
    }
  });

  glfwSetKeyCallback(window, [](GLFWwindow *, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
      app.onEvent(UiEvent(UiEventType::KEY, action == GLFW_PRESS, key));
    }
  });

  glfwMakeContextCurrent(window);

  gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

  // const GLubyte *version = glGetString(GL_VERSION);
  // const GLubyte *renderer = glGetString(GL_RENDERER);
  // const GLubyte *vendor = glGetString(GL_VENDOR);
  // std::cout << "OpenGL version: " << version << std::endl << "Renderer: " << renderer << std::endl << "Vendor: " <<
  //     vendor << std::endl;

  FT_Error ftErr = FT_Init_FreeType(&freetype);
  if (ftErr) {
    std::cout << "Failed to initialize freetype." << std::endl;
    return;
  }

  stbi_set_flip_vertically_on_load(true);

  renderUi.init();
  renderScene.init();
  renderSceneDef.init();
  renderFont.init(freetype);

  app.onResize();

  app.setScreen(std::make_shared<ScreenMain>());
}

void Application::onResize() {
  if (width == 0 || height == 0) return;

  glViewport(0, 0, width, height);

  renderUi.resize(width, height);
  renderScene.resize(width, height);
  renderSceneDef.resize(width, height);
  renderFont.resize(width, height);

  if (currentUi != nullptr) {
    currentUi->setBounds(0, 0, width, height);
    currentUi->layout();
  }
}

void Application::runApp() {
  initApp();

  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

  double fpsQueue = 1;

  auto lastIteration = std::chrono::high_resolution_clock::now();
  auto lastFrame = std::chrono::high_resolution_clock::now();
  auto lastFpsUpdate = std::chrono::high_resolution_clock::now();

  while (!glfwWindowShouldClose(window)) {
    auto currentIteration = std::chrono::high_resolution_clock::now();
    double loopDt = std::chrono::duration<double>(currentIteration - lastIteration).count();
    lastIteration = currentIteration;
    fpsQueue = std::min(fpsQueue + loopDt * targetFps, static_cast<double>(maxFpsQueue));

    while (fpsQueue > 1) {
      fpsQueue--;

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      auto currentFrame = std::chrono::high_resolution_clock::now();

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
  }
}

void Application::later(const std::function<void()> &task) {
  std::lock_guard lk(this->mtx);
  laterVec.push_back(task);
}

void Application::onEvent(UiEvent event) {
  if (currentScene != nullptr) currentScene->handle(event);
  if (currentUi != nullptr) currentUi->handle(event);

  if (event.type == UiEventType::KEY && event.down && event.button == GLFW_KEY_F11) {
    GLFWmonitor *mon = glfwGetWindowMonitor(window);
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

void Application::setScreen(const std::shared_ptr<Ui> &screen) {
  currentUi = screen;

  if (screen != nullptr) {
    currentUi->setBounds(0, 0, width, height);
    currentUi->layout();
  }
}

void Application::setScene(const std::shared_ptr<Scene> &scene) {
  if (currentScene != nullptr) {
    currentScene->close();
  }

  currentScene = scene;
  if (currentScene != nullptr) {
    currentScene->open();
    currentScene->resize(width, height);
  }
}

void Application::renderApp(double dt) {
  if (currentScene != nullptr) {
    currentScene->render(dt);
  }

  if (currentUi != nullptr) {
    currentUi->render(dt);
  }

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
