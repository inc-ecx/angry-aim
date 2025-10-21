#include "SceneDrill.h"

#include "Application.h"
#include "drill_controllers/FactoryDrill.h"
#include "entities/MainPlayer.h"
#include "entities/SimpleTarget.h"
#include "entities/StrafingTarget.h"
#include "screens/ScreenDrill.h"
#include "world_controllers/cam_consts.h"

SceneDrill::SceneDrill(const Drill &drill) :
  drill(drill) {
  targetModel = std::make_shared<Model>("assets/models/target1/Target1.obj");
  worldModel = std::make_shared<Model>("assets/models/map1/Map1.obj");
}

void SceneDrill::setup() {
  world = std::make_shared<WorldController>();
  world->add(player = std::make_shared<MainPlayer>());
  player->pos = glm::vec3(0.0f, 1.6f, 0.0f);
  screen = std::make_shared<ScreenDrill>();
  auto &app = Application::app;
  app.setScreen(screen);
  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  drillController->setup(world, player, screen);
}

void SceneDrill::replay() {
  drillController = FactoryDrill::create(drill);
  setup();
}

void SceneDrill::open() {
  drillController = FactoryDrill::create(drill);
  setup();

  auto &app = Application::app;
  auto &renderScene = app.renderSceneDef;
  renderScene.updateProjection();
}

void SceneDrill::close() {
  auto &app = Application::app;
  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void SceneDrill::handle(const UiEvent &event) {
  world->handle(event);
}

//
// rendering code
//

void SceneDrill::render(double dt) {
  world->update(dt);

  drawWorld();
  drawCrosshair();
}

void SceneDrill::drawWorld() {
  Application &app = Application::app;
  RenderSceneDefault &renderScene = app.renderSceneDef;
  renderScene.start();

  double camPitch = player->pitch;
  double camYaw = player->yaw;

  // when a crosshair has an odd total width, the crosshair is not aligned in the center of the screen.
  // it is offset 0.5 pixels to the top left. as the crosshair position is rounded down.
  // to make the crosshair accurate and pixel true the view is shifted by that amount.
  // note: this assumes the screen resolution is even.
  int chSize = ch.length + ch.gap + ch.length;
  if (chSize % 2 == 1) {
    camPitch += camSens * camPxToDeg;
    camYaw += camSens * camPxToDeg;
  }

  renderScene.updateView(
    player->pos,
    static_cast<float>(camPitch),
    static_cast<float>(camYaw)
  );

  renderScene.updateModel(glm::vec3(0, 0, 0), 1.0);
  renderScene.color(0xffffffff);
  renderScene.texture(true);
  renderScene.draw(*worldModel);

  for (auto e: world->world.entities) {
    auto target = std::dynamic_pointer_cast<SimpleTarget>(e);
    if (target == nullptr) continue;
    renderScene.updateModel(target->pos, static_cast<float>(0.5 * (target->size)));
    renderScene.color(0xf04040ff);
    renderScene.draw(*targetModel); // note: target model is of radius 1
  }

  for (auto e: world->world.entities) {
    auto target = std::dynamic_pointer_cast<StrafingTarget>(e);
    if (target == nullptr) continue;
    renderScene.updateModel(target->pos, static_cast<float>(0.5 * (target->size)));
    renderScene.color(0xf04040ff);
    renderScene.draw(*targetModel);
  }

  renderScene.texture(false);

  renderScene.stop();
}

void SceneDrill::drawCrosshair() {
  Application &app = Application::app;
  RenderUi &renderUi = app.renderUi;
  renderUi.start();
  renderUi.color(0x00ffffff);
  int thickness = ch.thickness;
  int length = ch.length;
  int gap = ch.gap;

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
