#include "SceneBackpack.h"

#include "../Application.h"

SceneBackpack::SceneBackpack() {
  world = std::make_shared<World>();
  model = std::make_shared<Model>("assets/models/backpack/backpack.obj");

  world->player.pos.z = 4;
}

SceneBackpack::~SceneBackpack() {
}

void SceneBackpack::render(double dt) {
  Application &app = Application::app;
  RenderScene &renderScene = app.renderScene;

  double speed = 6;
  glm::vec3 movement(0,0,0);
  if (glfwGetKey(app.window, GLFW_KEY_W) == GLFW_PRESS) movement.z--;
  if (glfwGetKey(app.window, GLFW_KEY_S) == GLFW_PRESS) movement.z++;
  if (glfwGetKey(app.window, GLFW_KEY_A) == GLFW_PRESS) movement.x--;
  if (glfwGetKey(app.window, GLFW_KEY_D) == GLFW_PRESS) movement.x++;
  if (glfwGetKey(app.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) movement.y--;
  if (glfwGetKey(app.window, GLFW_KEY_SPACE) == GLFW_PRESS) movement.y++;
  double l = glm::length(movement);
  if (l > 0.1) {
    float yaw = world->player.yaw;
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::vec3 rotated = glm::vec3(rot * glm::vec4(movement, 1.0f));
    world->player.pos += rotated * static_cast<float>(dt * speed / l);
  }

  renderScene.start();
  renderScene.updateView(world->player.pos, world->player.pitch, world->player.yaw);
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
  if (event.type == UiEventType::MOUSE_MOVE) {
    double sensitivity = 0.38 * 2;
    double degYaw = event.mdx * sensitivity * 0.07;
    double degPitch = event.mdy * sensitivity * 0.07;

    world->player.yaw = std::remainder(world->player.yaw + degYaw, 360);
    world->player.pitch = std::clamp(world->player.pitch + degPitch, -90.0, 90.0);
  }
}
