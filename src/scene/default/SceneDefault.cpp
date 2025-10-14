#include "SceneDefault.h"

#include "Application.h"
#include "util/math_util.h"
#include <format>

SceneDefault::SceneDefault() {
  world = std::make_shared<World>();
  targetModel = std::make_shared<Model>("assets/models/target1/Target1.obj");
  worldModel = std::make_shared<Model>("assets/models/map1/Map1.obj");

  world->player.pos = glm::vec3(0, 1.60, 0);

  std::shared_ptr<StrafingTarget> t1;
  world->targets.push_back(t1 = std::make_shared<StrafingTarget>());
  t1->min = glm::vec3(-4.0f, 1.5f, -20.0f);
  t1->max = glm::vec3(4.0f, 1.5f, -20.0f);
  t1->pos = glm::vec3(0.0f, 1.5f, -20.0f);
}

SceneDefault::~SceneDefault() {
}

void SceneDefault::drawCrosshair() {
  Application &app = Application::app;
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

void SceneDefault::drawWorld() {
  Application &app = Application::app;
  RenderSceneDefault &renderScene = app.renderSceneDef;
  renderScene.start();
  renderScene.updateView(world->player.pos, world->player.pitch, world->player.yaw);

  renderScene.updateModel(glm::vec3(0, 0, 0), 1.0);
  renderScene.color(0x606060ff);
  renderScene.draw(*worldModel);

  for (auto target: world->targets) {
    renderScene.updateModel(target->pos, 0.5 * target->size);
    renderScene.color(0xd03030ff);
    renderScene.draw(*targetModel);
  }

  renderScene.stop();
}

void SceneDefault::updateMovement(double dt) {
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
    float yaw = world->player.yaw;
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), -glm::radians(yaw), glm::vec3(0, 1, 0));
    glm::vec3 rotated = glm::vec3(rot * glm::vec4(movement, 1.0f));
    world->player.pos += rotated * static_cast<float>(dt * speed / l);
  }
}

void SceneDefault::updateWorld(double dt) {
  uint64_t msNow = msCurrent();

  bool doForceDirection = false;
  float input = 0;

  for (auto target: world->targets) {
    target->velocity = std::clamp(target->velocity + strafeAcceleration * target->input * dt, -strafeMaxSpeed,
                                  strafeMaxSpeed);

    bool decelerating = std::abs(target->input) < 0.1 && std::abs(target->velocity) > 0.1;
    if (decelerating) {
      float velocityDir = target->velocity < 0 ? -1.0 : 1.0;
      target->velocity = sub_nzc(target->velocity, velocityDir * dt * strafeDeceleration);
    }
  }

  for (auto target: world->targets) {
    glm::vec3 direction = glm::normalize(target->max - target->min);
    target->pos += target->velocity * direction * static_cast<float>(dt);

    // note: targets might become unaligned after a while, triggering a switch every frame.
    glm::vec3 overshoot = target->pos - target->max;
    if (overshoot.x + overshoot.y + overshoot.z > 0.3) {
      doForceDirection = true;
      input = -1;
    }
    glm::vec3 undershoot = target->min - target->pos;
    if (undershoot.x + undershoot.y + undershoot.z > 0.3) {
      doForceDirection = true;
      input = 1;
    }
  }

  // update strafe direction
  if (!world->targets.empty() && msNow > strafeMsSwitch || doForceDirection) {
    std::uniform_int_distribution inputFunc(-1, 1);
    int randomInput = inputFunc(strafeRandom);
    world->targets.front()->input = randomInput;
    if (doForceDirection && randomInput != 0) world->targets.front()->input = input;

    std::uniform_int_distribution durationFunc(strafeMinDurationMs, strafeMaxDurationMs);
    strafeMsSwitch = msNow + durationFunc(strafeRandom);
  }
}

void SceneDefault::render(double dt) {
  updateMovement(dt);

  updateWorld(dt);

  drawWorld();

  drawCrosshair();
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
    double sensitivity = 0.38;
    double degYaw = event.mdx * sensitivity * 0.07;
    double degPitch = event.mdy * sensitivity * 0.07;

    world->player.yaw = std::remainder(world->player.yaw + degYaw, 360);
    world->player.pitch = std::clamp(world->player.pitch + degPitch, -90.0, 90.0);
  }
}
