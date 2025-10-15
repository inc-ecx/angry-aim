#include "SceneDefault.h"

#include "Application.h"
#include "util/math_util.h"
#include <format>

#include "screens/ScreenScene.h"

SceneDefault::SceneDefault() {
  rng.seed(static_cast<uint32_t>(msCurrent()));

  world = std::make_shared<World>();
  targetModel = std::make_shared<Model>("assets/models/target1/Target1.obj");
  worldModel = std::make_shared<Model>("assets/models/map1/Map1.obj");

  world->player.pos = glm::vec3(0.0f, 1.6f, 0.0f);

  //std::shared_ptr<StrafingTarget> t1;
  //world->targets.push_back(t1 = std::make_shared<StrafingTarget>());
  //t1->min = glm::vec3(-4.0f, 1.6f, -10.0f);
  //t1->max = glm::vec3(4.0f, 1.6f, -10.0f);
  //t1->pos = glm::vec3(0.0f, 1.6f, -10.0f);
}

SceneDefault::~SceneDefault() {
}

void SceneDefault::triggerDone() {
  done = true;
}

void SceneDefault::drawCrosshair() {
  Application &app = Application::app;
  RenderUi &renderUi = app.renderUi;
  renderUi.start();
  renderUi.color(0x00ffffff);
  int thickness = 2;
  int length = 4;
  int gap = 4;

  ScreenScene &scene = *dynamic_cast<ScreenScene *>(app.getScreen().get());
  if (isRunning()) {
    uint64_t runningMs = msCurrent() - msStarted;
    int timeLeft = static_cast<int>((duration * 1000 - static_cast<int>(runningMs)) / 1000.0f + 0.5f);
    if (timeLeft > 0) {
      int m = timeLeft / 60;
      int s = timeLeft % 60;
      scene.lblMainStat->setText(std::format("{:02d}:{:02d}", m, s));
    } else {
      scene.lblMainStat->setText("00:00");
      if (!done) triggerDone();
    }
  } else {
    scene.lblMainStat->setText("Click to start");
  }

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
  renderScene.updateView(world->player.pos, static_cast<float>(world->player.pitch), static_cast<float>(world->player.yaw));

  renderScene.updateModel(glm::vec3(0, 0, 0), 1.0);
  renderScene.color(0xffffffff);
  renderScene.texture(true);
  renderScene.draw(*worldModel);

  for (auto target: world->targets) {
    renderScene.updateModel(target->pos, static_cast<float>(0.5 * (target->size + 0.05)));
    renderScene.color(0xf04040ff);
    renderScene.draw(*targetModel);
  }
  renderScene.texture(false);

  renderScene.stop();
}

void SceneDefault::updateMovement(double dt) {
  // Application &app = Application::app;
  // double speed = 6;
  // glm::vec3 movement(0, 0, 0);
  // if (glfwGetKey(app.window, GLFW_KEY_W) == GLFW_PRESS) movement.z--;
  // if (glfwGetKey(app.window, GLFW_KEY_S) == GLFW_PRESS) movement.z++;
  // if (glfwGetKey(app.window, GLFW_KEY_A) == GLFW_PRESS) movement.x--;
  // if (glfwGetKey(app.window, GLFW_KEY_D) == GLFW_PRESS) movement.x++;
  // if (glfwGetKey(app.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) movement.y--;
  // if (glfwGetKey(app.window, GLFW_KEY_SPACE) == GLFW_PRESS) movement.y++;
  // double l = glm::length(movement);
  // if (l > 0.1) {
  //   float yaw = static_cast<float>(world->player.yaw);
  //   glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0, 1, 0));
  //   glm::vec3 rotated = glm::vec3(rot * glm::vec4(movement, 1.0f));
  //   world->player.pos += rotated * static_cast<float>(dt * speed / l);
  // }
}

void SceneDefault::updateWorld(double dt) {
  uint64_t msNow = msCurrent();

  bool doForceDirection = false;
  float input = 0;

  // apply acceleration/deceleration
  for (auto target: world->targets) {
    target->strafeVelocity = static_cast<float>(std::clamp(target->strafeVelocity + strafeAcceleration * target->strafeInput * dt, -strafeMaxSpeed,
                                  strafeMaxSpeed));

    bool decelerating = std::abs(target->strafeInput) < 0.1 && std::abs(target->strafeVelocity) > 0.01;
    if (decelerating) {
      float velocityDir = target->strafeVelocity < 0 ? -1.0f : 1.0f;
      target->strafeVelocity = sub_nzc(target->strafeVelocity, static_cast<float>(velocityDir * dt * strafeDeceleration));
    }
  }

  // apply velocity
  for (auto target : world->targets) {
    target->strafePos += target->strafeVelocity * static_cast<float>(dt);
    target->pos = target->strafeStart + glm::vec3(target->strafePos, 0, 0);
  }

  // steer back to original position if max strafe width is hit
  for (auto target: world->targets) {
    float overshoot = target->strafePos - target->strafeWidth * 0.5f;
    if (overshoot > 0.3) {
      doForceDirection = true;
      input = -1;
    }
    float undershoot = - target->strafeWidth * 0.5f - target->strafePos;
    if (undershoot > 0.3) {
      doForceDirection = true;
      input = 1;
    }
  }

  if (isRunning()) {
    // update strafe direction
    if (!world->targets.empty() && msNow > strafeMsSwitch || doForceDirection) {
      std::uniform_int_distribution inputFunc(-1, 1);
      int randomInput = inputFunc(rng);
      world->targets.front()->strafeInput = static_cast<float>(randomInput);
      if (doForceDirection && randomInput != 0) world->targets.front()->strafeInput = input;

      std::uniform_int_distribution durationFunc(strafeMinDurationMs, strafeMaxDurationMs);
      strafeMsSwitch = msNow + durationFunc(rng);
    }
  }
}

void SceneDefault::start() {
  started = true;
  msStarted = msCurrent();
  triggerSpawn();
}

void SceneDefault::render(double dt) {
  // DebugInfo::put("yaw", std::format("{:.0f}", world->player.yaw));
  // DebugInfo::put("pitch", std::format("{:.0f}", world->player.pitch));

  updateMovement(dt);

  updateWorld(dt);

  drawWorld();

  drawCrosshair();

  uint64_t msNow = msCurrent();
  if (spawnQueue > 0 && static_cast<int64_t>(msNow - spawnMsSpawn) >= 0) {
    float mapWidth = 20;
    std::uniform_real_distribution<float> randomFloat;

    float strafeHeight = 1.5f;
    float strafeDist = spawnMinDist + randomFloat(rng) * (spawnMaxDist - spawnMinDist);
    float strafeWidth = spawnMinStrafe + randomFloat(rng) * (spawnMaxStrafe - spawnMinStrafe);

    float spawnSpace = mapWidth - strafeWidth;
    float spawnX = randomFloat(rng) * spawnSpace - spawnSpace * 0.5f;

    auto target = std::make_shared<StrafingTarget>();
    target->strafeStart = {spawnX, strafeHeight, -strafeDist};
    target->strafeWidth = strafeWidth;

    world->targets.push_back(target);

    spawnQueue--;
  }
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

void SceneDefault::triggerSpawn() {
  spawnQueue++;
  spawnMsSpawn = msCurrent() + spawnDelay;
}

void SceneDefault::handle(const UiEvent &event) {
  if (event.type == UiEventType::MOUSE_MOVE) {
    double sensitivity = 0.38;
    double degYaw = -event.mdx * sensitivity * 0.07;
    double degPitch = -event.mdy * sensitivity * 0.07;

    world->player.yaw = std::remainder(world->player.yaw + degYaw, 360);
    world->player.pitch = std::clamp(world->player.pitch + degPitch, -90.0, 90.0);
  }

  if (event.type == UiEventType::MOUSE_BUTTON && event.button == GLFW_MOUSE_BUTTON_LEFT && event.down) {
    if (!isStarted()) {
      start();
      return;
    }

    for (auto it = world->targets.begin(); it != world->targets.end();) {
      float tHit = 0;
      bool didHit = hitSphere(world->player.pos, static_cast<float>(world->player.pitch), static_cast<float>(world->player.yaw), (*it)->pos, (*it)->size * 0.5f, tHit);
      if (didHit) {
        it = world->targets.erase(it);
        // ++it;
        triggerSpawn();
      } else {
        ++it;
      }
    }
  }
}

bool SceneDefault::hitSphere(const glm::vec3& rayOrigin, float pitch, float yaw,
                             const glm::vec3& spherePos, float radius, float& tHit)
{
  glm::vec3 dir;
  dir.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw + 180));
  dir.y = sin(glm::radians(pitch));
  dir.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw + 180));
  dir = glm::normalize(dir);

  glm::vec3 L = spherePos - rayOrigin;
  float tca = glm::dot(L, dir);
  if (tca < 0) return false; // Sphere is behind the ray

  float d2 = glm::dot(L, L) - tca * tca;
  float r2 = radius * radius;
  if (d2 > r2) return false; // Misses the sphere

  float thc = sqrt(r2 - d2);
  tHit = tca - thc; // nearest intersection along the ray
  return true;
}