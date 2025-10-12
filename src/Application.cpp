#include "Application.h"

#include <iostream>
#include <chrono>
#include <algorithm>

#include "screens/ScreenMain.h"

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
    app.mouseX = static_cast<int>(x);
    app.mouseY = static_cast<int>(y);
  });
  double mx, my;
  glfwGetCursorPos(window, &mx, &my);
  mouseX = mx;
  mouseY = my;

  glfwSetWindowSizeCallback(window, [](GLFWwindow *, int w, int h) {
    app.width = w;
    app.height = h;
    app.onResize();
  });

  glfwMakeContextCurrent(window);

  gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

  const GLubyte *version = glGetString(GL_VERSION);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *vendor = glGetString(GL_VENDOR);
  std::cout << "OpenGL version: " << version << std::endl << "Renderer: " << renderer << std::endl << "Vendor: " << vendor << std::endl;

  FT_Error ftErr = FT_Init_FreeType(&freetype);
  if (ftErr) {
    std::cout << "Failed to initialize freetype." << std::endl;
    return;
  }

  renderUi.init();
  renderFont.init(freetype);

  currentUi = std::make_unique<ScreenMain>();

  app.onResize();
}

void Application::onResize() {
  glViewport(0,0,width,height);

  renderUi.resize(width, height);
  renderFont.resize(width,height);

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

  while (!glfwWindowShouldClose(window)) {
    auto currentIteration = std::chrono::high_resolution_clock::now();
    double dt = std::chrono::duration<double>(currentIteration - lastIteration).count();
    lastIteration = currentIteration;
    fpsQueue = std::min(fpsQueue + dt * targetFps, static_cast<double>(maxFpsQueue));

    while (fpsQueue > 1) {
      fpsQueue--;

      glClear(GL_COLOR_BUFFER_BIT);

      renderApp();

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
  }
}

void Application::renderApp() {
  if (currentUi != nullptr) {
    currentUi->render();
  }


  renderFont.start();
  renderFont.renderText("When the imposter is sus. AY Ay T.", 40,40, 1.0f, glm::vec4(1,1,1,1));
  renderFont.stop();
}
