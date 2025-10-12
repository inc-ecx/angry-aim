#include "Scene.h"

#include "../Application.h"

Scene::Scene() {
}

void Scene::render() {
  Application &app = Application::app;
  RenderScene &renderScene = app.renderScene;

  renderScene.start();
  renderScene.color(0x00ffffff);
  renderScene.rect(0,0,50,50);
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
