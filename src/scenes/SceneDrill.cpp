#include "SceneDrill.h"

#include "Application.h"
#include "Log.h"
#include "drill_controllers/FactoryDrill.h"
#include "entities/MainPlayer.h"
#include "entities/SimpleTarget.h"
#include "entities/StrafingTarget.h"
#include "renderers/RenderCrosshair.h"
#include "scenes/UiDrill.h"
#include "screens/ScreenPause.h"

SceneDrill::SceneDrill(const Drill &drill) :
  drill(drill) {
  targetModel = std::make_shared<Model>("assets/models/target1/Target1.obj");
  worldModel = std::make_shared<Model>("assets/models/map1/Map1.obj");

  auto &app = Application::app;
  resources = std::make_shared<SceneDrillResources>();
  resources->soundHit = app.loadSound("assets/sounds/ding.wav");
  resources->soundMiss = app.loadSound("assets/sounds/click.wav");

  observations.push_back(State::state.settings.listenChangeView(std::bind(&SceneDrill::viewSettingsChanged, this)));
}

void SceneDrill::viewSettingsChanged() {
  auto &renderScene = Application::app.renderSceneDrill;
  renderScene.updateProjection();
}

void SceneDrill::setup() {
  auto &app = Application::app;

  // create world
  world = std::make_shared<WorldController>();
  world->add(player = std::make_shared<MainPlayer>());
  player->pos = glm::vec3(0.0f, 1.6f, 0.0f);

  // create ui
  ui = std::make_shared<UiDrill>();
  ui->setBounds(0, 0, app.getWidth(), app.getHeight());
  ui->layout();

  // create controller
  drillController = FactoryDrill::create(drill);

  drillController->setup(DrillControllerSetupArgs{
    .world = world,
    .player = player,
    .screen = ui,
    .resources = resources
  });
}

//
// drill code
//

void SceneDrill::replay() {
  auto &app = Application::app;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  setup();
}

void SceneDrill::pause() {
  if (!isInGame()) {
    Log::warn("Tried to pause game, but we are not in fact in-game.");
    return;
  }

  auto &app = Application::app;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  drillController->pause();

  app.later(
    [&app] {
      auto scene = dynamic_pointer_cast<SceneDrill>(app.getScene());
      if (!scene) return;
      app.setScreen(std::make_shared<ScreenPause>(scene));
    }
  );
}

void SceneDrill::resume() {
  auto &app = Application::app;
  auto &renderScene = app.renderSceneDrill;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  drillController->resume();

  renderScene.updateProjection();
}

//
// scene code
//

void SceneDrill::open() {
  auto &app = Application::app;
  auto &renderScene = app.renderSceneDrill;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  setup();

  renderScene.updateProjection();
}

void SceneDrill::close() {
  auto &app = Application::app;

  glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void SceneDrill::handle(const UiEvent &event) {
  if (isInGame()) {
    world->handle(event);

    UiEvent e = event;
    ui->handle(e);

    if (event.type == UiEventType::KEY && event.down) {
      if (event.button == GLFW_KEY_ESCAPE) {
        pause();
      }
    }
  }
}

void SceneDrill::resize(int width, int height) {
  ui->setBounds(0, 0, width, height);
  ui->layout();
}

//
// rendering code
//

void SceneDrill::render(double dt) {
  world->update(dt);

  drawWorld();
  drawCrosshair();

  ui->render(dt, {.toBuffer = false});

  if (!isInGame()) renderBackground(dt);
}

void SceneDrill::renderBackground(double dt) {
  auto &app = Application::app;
  auto &render = app.renderUi;

  render.start();
  render.color(0x30303060);
  render.rect(0, 0, app.getWidth(), app.getHeight());
  render.stop();
}

void SceneDrill::drawWorld() {
  Application &app = Application::app;
  RenderSceneDrill &renderScene = app.renderSceneDrill;
  renderScene.start();

  double camPitch = player->pitch;
  double camYaw = player->yaw;

  // TODO: this does not work, because the raytrace would still be inaccurate
  // when a crosshair has an odd total width, the crosshair is not aligned in the center of the screen.
  // it is offset 0.5 pixels to the top left. as the crosshair position is rounded down.
  // to make the crosshair accurate and pixel true the view is shifted by that amount.
  // note: this assumes the screen resolution is even.
  //int chSize = ch.length + ch.gap + ch.length;
  //if (chSize % 2 == 1) {
  //  camPitch += 0.5*pxToDeg();
  //  camYaw += 0.5*pxToDeg();
  //}

  renderScene.updateView(
    player->pos,
    static_cast<float>(camPitch),
    static_cast<float>(camYaw),
    glm::vec2(100,100)
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

  // RenderCrosshair::render(app.getWidth() / 2 - 50, app.getHeight() / 2 - 50, 0, 0); // debug
  RenderCrosshair::render(0, 0, app.getWidth(), app.getHeight());
}
