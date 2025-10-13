#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include "Mesh.h"
#include "Model.h"
#include "../inc/inc_glfw.h"

class RenderScene {
  GLuint shaderProgram = -1;

  GLuint u_model = -1;
  GLuint u_view = -1;
  GLuint u_projection = -1;

  void initRectVao();

  void initShader();

public:

  void init();

  void start();

  void stop();

  void mesh(const Mesh& mesh);

  void model(const Model& model);

  void resize(int width, int height);
};

#endif //RENDER_SCENE_H
