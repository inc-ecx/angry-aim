#include "MoveController.h"

#include "Application.h"

MoveController::MoveController(
  const std::shared_ptr<Positioned> &positioned,
  const std::shared_ptr<Looking> &looking
)
  : Toggelable(true),
    positioned(positioned),
    looking(looking) {
}

void MoveController::update(double dt) {
  Application &app = Application::app;

  double speed = 6;
  glm::vec3 movement(0, 0, 0);
  if (glfwGetKey(app.window, GLFW_KEY_W) == GLFW_PRESS) movement.z--;
  if (glfwGetKey(app.window, GLFW_KEY_S) == GLFW_PRESS) movement.z++;
  if (glfwGetKey(app.window, GLFW_KEY_A) == GLFW_PRESS) movement.x--;
  if (glfwGetKey(app.window, GLFW_KEY_D) == GLFW_PRESS) movement.x++;
  if (glfwGetKey(app.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) movement.y--;
  if (glfwGetKey(app.window, GLFW_KEY_SPACE) == GLFW_PRESS) movement.y++;
  double l = glm::length(movement);
  if (l > 0.1) {
    float yaw = static_cast<float>(looking->yaw);
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::vec3 rotated = glm::vec3(rot * glm::vec4(movement, 1.0f));
    positioned->pos += rotated * static_cast<float>(dt * speed / l);
  }
}
