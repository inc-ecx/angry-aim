#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include "../inc/inc_glfw.h"

class RenderScene {
  GLuint rectVao = -1;
  GLuint shaderProgram = -1;

  GLuint u_baseColor = -1;
  GLuint u_model = -1;
  GLuint u_projection = -1;

  void initRectVao();

  void initShader();

public:

  void init();

  void start();

  void stop();

  void color(int rgba);

  void rect(int x, int y, int w, int h);

  void resize(int width, int height);
};

#endif //RENDER_SCENE_H
