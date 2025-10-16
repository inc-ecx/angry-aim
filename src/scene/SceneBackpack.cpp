#include "SceneBackpack.h"

#include "../Application.h"
#include "controllers/CameraController.h"
#include "controllers/MoveController.h"
#include "entities/MainPlayer.h"

SceneBackpack::SceneBackpack() {
  controller = std::make_shared<Controller>();
  model = std::make_shared<Model>("assets/models/backpack/backpack.obj");

  controller->add(player = std::make_shared<MainPlayer>());
  controller->control(player, std::make_shared<CameraController>(player));
  controller->control(player, std::make_shared<MoveController>(player, player));
  player->pos.z = 4;
}

SceneBackpack::~SceneBackpack() {
}

void SceneBackpack::render(double dt) {
  controller->update(dt);

  Application &app = Application::app;
  RenderScene &renderScene = app.renderScene;
  renderScene.start();
  renderScene.updateView(player->pos, static_cast<float>(player->pitch), static_cast<float>(player->yaw));
  renderScene.model(*model);
  renderScene.stop();
}

void SceneBackpack::open() {
  Application &app = Application::app;
  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void SceneBackpack::close() {
  Application &app = Application::app;
  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void SceneBackpack::resize(int width, int height) {

}

void SceneBackpack::handle(const UiEvent &event) {
  controller->handle(event);
}
