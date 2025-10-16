#include "SceneDefault.h"

#include "../Application.h"
#include "../util/math_util.h"
#include <format>

#include "../entities/StrafingTarget.h"
#include "../screens/ScreenResult.h"
#include "../screens/ScreenScene.h"
#include "controllers/CameraController.h"
#include "controllers/MoveController.h"

SceneDefault::SceneDefault() {
  rng.seed(static_cast<uint32_t>(msCurrent()));

  // load models
  targetModel = std::make_shared<Model>("assets/models/target1/Target1.obj");
  worldModel = std::make_shared<Model>("assets/models/map1/Map1.obj");

  // load world
  controller = std::make_shared<Controller>();
  controller->add(player = std::make_shared<MainPlayer>());
  controller->control(player, std::make_shared<CameraController>(player));
  controller->control(player, std::make_shared<MoveController>(player, player));
  player->pos = glm::vec3(0.0f, 1.6f, 0.0f);
}

SceneDefault::~SceneDefault() {
}

void SceneDefault::triggerOver() {
  over = true;
  auto &app = Application::app;
  int m = duration / 60;
  int s = duration % 60;
  std::vector stats{
    std::format("Time: {:02d}:{:02d}", m, s),
    std::format("Hit: {}", statsHit),
    std::format("TTK: {}ms", ttkSum / statsHit),
    std::format("Hit Rate: {:0.1f}%", static_cast<float>(statsHit) / (statsHit + statsMiss) * 100.0f),
  };
  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  app.later([stats] {
    Application::app.setScreen(std::make_shared<ScreenResult>(stats));
  });
}

void SceneDefault::drawCrosshair() {
  Application &app = Application::app;
  RenderUi &renderUi = app.renderUi;
  renderUi.start();
  renderUi.color(0x00ffffff);
  int thickness = 2;
  int length = 4;
  int gap = 4;

  ScreenScene *scene = dynamic_cast<ScreenScene *>(app.getScreen().get());
  if (scene != nullptr) {
    if (isStarted()) {
      uint64_t runningMs = msCurrent() - msStarted;
      int timeLeft = static_cast<int>((duration * 1000 - static_cast<int>(runningMs)) / 1000.0f + 0.5f);
      if (timeLeft > 0) {
        int m = timeLeft / 60;
        int s = timeLeft % 60;
        scene->lblMainStat->setText(std::format("{:02d}:{:02d}", m, s));
      } else {
        scene->lblMainStat->setText("00:00");
        if (!over) triggerOver();
      }
    } else {
      scene->lblMainStat->setText("Click to start");
    }
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
  renderScene.updateView(player->pos, static_cast<float>(player->pitch),
                         static_cast<float>(player->yaw));

  renderScene.updateModel(glm::vec3(0, 0, 0), 1.0);
  renderScene.color(0xffffffff);
  renderScene.texture(true);
  renderScene.draw(*worldModel);

  for (auto e: controller->world.entities) {
    StrafingTarget* target = dynamic_cast<StrafingTarget*>(e.get());
    if (target == nullptr) continue;

    renderScene.updateModel(target->pos, static_cast<float>(0.5 * (target->size + 0.05)));
    renderScene.color(0xf04040ff);
    renderScene.draw(*targetModel);
  }
  renderScene.texture(false);

  renderScene.stop();
}

void SceneDefault::updateWorld(double dt) {
  if (!isRunning()) return;

  uint64_t msNow = msCurrent();

  bool doForceDirection = false;
  float input = 0;

  // apply acceleration/deceleration
  for (auto e: controller->world.entities) {
    StrafingTarget* target = dynamic_cast<StrafingTarget*>(e.get());
    if (target == nullptr) continue;

    target->strafeVelocity = static_cast<float>(std::clamp(
      target->strafeVelocity + strafeAcceleration * target->strafeInput * dt, -strafeMaxSpeed,
      strafeMaxSpeed));

    bool decelerating = std::abs(target->strafeInput) < 0.1 && std::abs(target->strafeVelocity) > 0.01;
    if (decelerating) {
      float velocityDir = target->strafeVelocity < 0 ? -1.0f : 1.0f;
      target->strafeVelocity = sub_nzc(target->strafeVelocity,
                                       static_cast<float>(velocityDir * dt * strafeDeceleration));
    }
  }

  // apply velocity
  for (auto e: controller->world.entities) {
    StrafingTarget* target = dynamic_cast<StrafingTarget*>(e.get());
    if (target == nullptr) continue;

    target->strafePos += target->strafeVelocity * static_cast<float>(dt);
    target->pos = target->strafeStart + glm::vec3(target->strafePos, 0, 0);
  }

  StrafingTarget* t = nullptr;

  // steer back to original position if max strafe width is hit
  for (auto e: controller->world.entities) {
    StrafingTarget* target = dynamic_cast<StrafingTarget*>(e.get());
    if (target == nullptr) continue;
    t = target;

    float overshoot = target->strafePos - target->strafeWidth * 0.5f;
    if (overshoot > 0.3) {
      doForceDirection = true;
      input = -1;
    }
    float undershoot = -target->strafeWidth * 0.5f - target->strafePos;
    if (undershoot > 0.3) {
      doForceDirection = true;
      input = 1;
    }
  }

  // update strafe direction
  if (t != nullptr && (msNow > strafeMsSwitch || doForceDirection)) {
    std::uniform_int_distribution inputFunc(-1, 1);
    int randomInput = inputFunc(rng);
    t->strafeInput = static_cast<float>(randomInput);
    if (doForceDirection && randomInput != 0) t->strafeInput = input;

    std::uniform_int_distribution durationFunc(strafeMinDurationMs, strafeMaxDurationMs);
    strafeMsSwitch = msNow + durationFunc(rng);
  }
}

void SceneDefault::start() {
  started = true;
  msStarted = msCurrent();
  triggerSpawn();
}

void SceneDefault::render(double dt) {
  // DebugInfo::put("yaw", std::format("{:.0f}", player->yaw));
  // DebugInfo::put("pitch", std::format("{:.0f}", player->pitch));

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

    controller->add(target);
    msLastSpawn = msCurrent();

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
  if (!isStarted()) {
    if (event.type == UiEventType::MOUSE_BUTTON && event.button == GLFW_MOUSE_BUTTON_LEFT && event.down) {
      start();
    }
  }

  if (!over) {
    controller->handle(event);
  }

  if (isRunning()) {
    if (event.type == UiEventType::MOUSE_BUTTON && event.button == GLFW_MOUSE_BUTTON_LEFT && event.down) {
      bool hit = false;
      std::vector<std::shared_ptr<StrafingTarget>> targetsIg;
      for (auto e: controller->world.entities) {
        auto strafingTarget = std::dynamic_pointer_cast<StrafingTarget>(e);
        if (strafingTarget) targetsIg.push_back(strafingTarget);
      }
      for (auto target : targetsIg) {
        float tHit = 0;
        bool didHit = hitSphere(player->pos, static_cast<float>(player->pitch),
                                static_cast<float>(player->yaw), target->pos, target->size * 0.5f, tHit);
        if (didHit) {
          controller->remove(target);
          // ++it;
          hit = true;
          statsHit++;
          ttkSum += static_cast<int>(msCurrent() - msLastSpawn);
          triggerSpawn();
        }
      }
      if (!hit) statsMiss++;
    }
  }
}

bool SceneDefault::hitSphere(const glm::vec3 &rayOrigin, float pitch, float yaw,
                             const glm::vec3 &spherePos, float radius, float &tHit) {
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
