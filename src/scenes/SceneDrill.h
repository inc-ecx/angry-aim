#ifndef SCENE_DRILL_H
#define SCENE_DRILL_H

#include <memory>

#include "Scene.h"
#include "world/controller/WorldController.h"
#include "drill/controller/DrillController.h"
#include "drill/model/Drill.h"
#include "entities/MainPlayer.h"

#include "render/Model.h"
#include "scenes/UiDrill.h"

class SceneDrill : public Scene {
  Drill drill;
  std::shared_ptr<WorldController> world;
  std::shared_ptr<DrillController> drillController;
  std::shared_ptr<MainPlayer> player;

  std::shared_ptr<Model> targetModel;
  std::shared_ptr<Model> worldModel;

  // note: this ui is completely managed by the scene, as this allows for the application screen to overlay it.
  std::shared_ptr<UiDrill> ui;

public:
  explicit SceneDrill(const Drill& drill);

  void setup();

  void resume();

  void replay() override;

  void open() override;

  void close() override;

  void handle(const UiEvent &event) override;

  void resize(int width, int height) override;

  //
  // rendering code
  //

  void render(double dt) override;

private:
  void drawWorld();

  void drawCrosshair();

  //
};

#endif //SCENE_DRILL_H
