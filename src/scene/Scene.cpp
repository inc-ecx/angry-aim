#include "Scene.h"

#include "../Application.h"

Scene::Scene() {
  // model = std::make_shared<Model>("Model");
}

void Scene::render() {
  Application &app = Application::app;
  RenderScene &renderScene = app.renderScene;

  renderScene.start();
  renderScene.stop();
}

void Scene::open() {
}

void Scene::close() {
}

void Scene::resize(int width, int height) {
}

void Scene::handle(const UiEvent &event) {
}
