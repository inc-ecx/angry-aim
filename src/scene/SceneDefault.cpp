#include "SceneDefault.h"

#include "../Application.h"

SceneDefault::SceneDefault() {
  world = std::make_shared<World>();
  model = std::make_shared<Model>("assets/models/target1/Target1.obj");

  world->player.pos.z = 15;
}

SceneDefault::~SceneDefault() {
}

void SceneDefault::render(double dt) {
  Application &app = Application::app;
  RenderSceneDefault &renderScene = app.renderSceneDef;

  renderScene.start();
  renderScene.color(0xd03030ff);
  renderScene.updateView(world->player.pos, world->player.pitch, world->player.yaw);
  renderScene.model(*model);
  renderScene.stop();

  RenderUi &renderUi = app.renderUi;
  renderUi.start();
  renderUi.color(0x00ffffff);
  int thickness = 2;
  int length = 4;
  int gap = 4;

  // top left coords
  int chSize = length + gap + length;
  int tlx = static_cast<int>(round((app.getWidth() - chSize) / 2.0));
  int tly = static_cast<int>(round((app.getHeight() - chSize) / 2.0));

  // left
  renderUi.rect(tlx, static_cast<int>(tly + (chSize - thickness) / 2.0), length, thickness);
  // right
  renderUi.rect(tlx + chSize - length, static_cast<int>(tly + (chSize - thickness) / 2.0), length, thickness);
  // top
  renderUi.rect(static_cast<int>(tlx + (chSize - thickness) / 2.0), tly, thickness, length);
  // bottom
  renderUi.rect(static_cast<int>(tlx + (chSize - thickness) / 2.0), tly + chSize - length, thickness, length);

  renderUi.stop();
}

void SceneDefault::open() {
  Application &app = Application::app;
  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void SceneDefault::close() {
  Application &app = Application::app;
  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void SceneDefault::resize(int width, int height) {

}

void SceneDefault::handle(const UiEvent &event) {
  if (event.type == UiEventType::MOUSE_MOVE) {
    double sensitivity = 0.38 * 2;
    double degYaw = event.mdx * sensitivity * 0.07;
    double degPitch = event.mdy * sensitivity * 0.07;

    world->player.yaw = std::remainder(world->player.yaw + degYaw, 360);
    world->player.pitch = std::clamp(world->player.pitch + degPitch, -90.0, 90.0);
  }
}
