#ifndef RENDER_SCENE_DEFAULT_H
#define RENDER_SCENE_DEFAULT_H

#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "../inc/inc_glfw.h"

class RenderSceneDrill {
  glm::mat4 projection = glm::mat4();

  Shader shader;

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

  void updateModel(glm::vec3 pos, float scale);

  void updateView(glm::vec3 pos, float pitch, float yaw, glm::vec2 pxOffset);

  void updateProjection();
};


#endif //RENDER_SCENE_DEFAULT_H
