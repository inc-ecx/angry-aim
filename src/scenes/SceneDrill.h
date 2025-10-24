#ifndef SCENE_DRILL_H
#define SCENE_DRILL_H

#include <memory>

#include "Application.h"
#include "Scene.h"
#include "world/controller/WorldController.h"
#include "drill/controller/DrillController.h"
#include "drill/model/Drill.h"
#include "entities/MainPlayer.h"

#include "render/Model.h"
#include "scenes/UiDrill.h"

class SceneDrill : public Scene {

  // scene parameters
  Drill drill;

  // scene resources
  std::shared_ptr<Model> targetModel;
  std::shared_ptr<Model> worldModel;

  //
  // drill state
  //
  std::shared_ptr<WorldController> world;
  std::shared_ptr<DrillController> drillController;
  std::shared_ptr<MainPlayer> player;

  bool isInGame() {
    auto &app = Application::app;
    return app.getScreen() == nullptr;
  }

  // this ui is used for displaying the in-game hud.
  // note: this ui is completely managed by the scene, as this allows for the application screen to overlay it.
  std::shared_ptr<UiDrill> ui;

  // sets up world, ui and cursor input mode
  void setup();

public:
  explicit SceneDrill(const Drill &drill);

  //
  // drill specific
  //

  // resumes the drill. called by pause screen after closing.
  void resume();

  // pauses the drill. opens pause screen.
  void pause();

  //
  // scene specific
  //

  // restarts scene
  void replay() override;

  // sets up scenes resources
  void open() override;

  // frees scene resources
  void close() override;

  // ui event
  void handle(const UiEvent &event) override;

  // ui event
  void resize(int width, int height) override;

  //
  // rendering code
  //

  void render(double dt) override;

private:
  void renderBackground(double dt);

  void drawWorld();

  void drawCrosshair();

};

#endif //SCENE_DRILL_H
