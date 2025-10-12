#ifndef RENDER_UI_H
#define RENDER_UI_H

#include "../inc/inc_glfw.h"

class RenderUi {
  GLuint rectVao = -1;
  GLuint shaderProgram = -1;

  GLuint u_baseColor = -1;

public:
  void initRectVao();

  void initShader();

  void init();

  void start();

  void stop();

  void rect(int x, int y, int w, int h);

  void resize(int width, int height);
};

#endif //RENDER_UI_H
