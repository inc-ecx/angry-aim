#ifndef RENDER_SCENE_DEFAULT_H
#define RENDER_SCENE_DEFAULT_H


#include "Mesh.h"
#include "Model.h"
#include "../inc/inc_glfw.h"

class RenderSceneDefault {
  GLuint shaderProgram = -1;

  void initRectVao();

  void initShader();

public:
  void init();

  void start();

  void stop();

  void texture(bool value);

  void color(int rgba);

  void mesh(const Mesh &mesh);

  void draw(const Model &model);

  void resize(int width, int height);

  void updateView(glm::vec3 pos, float pitch, float yaw);

  void updateModel(glm::vec3 pos, float scale);
};


#endif //RENDER_SCENE_DEFAULT_H
